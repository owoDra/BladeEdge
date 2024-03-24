// Copyright (C) 2024 owoDra

#include "BEHostMigrationSubsystem.h"

#include "Experience/UserFacingExperienceData.h"
#include "RejoinMatch/BERejoinMatchSubsystem.h"
#include "ProjectBELogs.h"

// Game Online Core
#include "OnlineServiceSubsystem.h"
#include "OnlineLobbySubsystem.h"
#include "Type/OnlineLobbyCreateTypes.h"
#include "Type/OnlineLobbySearchTypes.h"

// Engine Feature
#include "Kismet/GameplayStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEHostMigrationSubsystem)


// Initialization

bool UBEHostMigrationSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (Cast<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		return false;
	}

	TArray<UClass*> ChildClasses;
	GetDerivedClasses(GetClass(), ChildClasses, false);

	// Only create an instance if there is not a game-specific subclass

	return ChildClasses.Num() == 0;
}

void UBEHostMigrationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	auto* OnlineLobbySubsystem{ Collection.InitializeDependency<UOnlineLobbySubsystem>() };
	check(OnlineLobbySubsystem);

	OnlineLobbySubsystem->OnLobbyBecomeLeader.AddUObject(this, &ThisClass::HandleBecomeLeader);
}

void UBEHostMigrationSubsystem::Deinitialize()
{
	if (auto* OnlineLobbySubsystem{ UGameInstance::GetSubsystem<UOnlineLobbySubsystem>(GetGameInstance()) })
	{
		OnlineLobbySubsystem->OnLobbyBecomeLeader.RemoveAll(this);
	}
}


void UBEHostMigrationSubsystem::HandleBecomeLeader(FName LocalName)
{
	if (LocalName == NAME_GameSession)
	{
		HandleHostMigrationRequest();
	}
}


// HostMigration

bool UBEHostMigrationSubsystem::HandleHostMigrationRequest()
{
	bRequested = true;

	return TryHostMigration();
}

bool UBEHostMigrationSubsystem::HandleHostMigrationRequestURL(const FString& InURL)
{
	HostMigrationRequestURL = InURL;

	return TryHostMigration();
}

bool UBEHostMigrationSubsystem::TryHostMigration()
{
	UE_LOG(LogBE_HostMigration, Log, TEXT("Try Host Migration"));
	UE_LOG(LogBE_HostMigration, Log, TEXT("| bRequested: %s"), bRequested ? TEXT("TRUE") : TEXT("FALSE"));
	UE_LOG(LogBE_HostMigration, Log, TEXT("| URL: %s"), *HostMigrationRequestURL);

	/// @TODO: OSSV2 のバグでLeaderChangeがうまくハンドルされないことがあるため一時的にLeaderChangeを考慮せずにホストマイグレーションを行う
	///		   このゲームでは1対1のゲームモードが基本なため問題はない

	//if (bRequested && !HostMigrationRequestURL.IsEmpty())
	if (!HostMigrationRequestURL.IsEmpty())
	{
		if (Step1_CheckLobbyExist())
		{
			return true;
		}

		ClearHostMigrationRequest();
	}

	return false;
}

void UBEHostMigrationSubsystem::ClearHostMigrationRequest()
{
	bRequested = false;
	HostMigrationRequestURL = FString();
}


bool UBEHostMigrationSubsystem::Step1_CheckLobbyExist()
{
	auto* World{ GetWorld() };

	if (ensure(World))
	{
		if (auto* OnlineServiceSubsystem{ UGameInstance::GetSubsystem<UOnlineServiceSubsystem>(World->GetGameInstance()) })
		{
			const auto CurrentService{ OnlineServiceSubsystem->GetOnlineServiceType() };

			// Online Service Null の場合はホストが切断すると即座にロビーが破棄されるのでロビーを再作成する

			if (CurrentService == EOnlineServiceType::Null)
			{
				return Step2_LeaveLobby();
			}

			// Online Service EOS の場合はホスト切断後にロビーにメンバーが残っていれば存続し続けるためロビーの再作成をスキップする

			else if (CurrentService == EOnlineServiceType::Epic)
			{
				return Step6_ServerTravel();
			}
		}
	}

	return false;
}

bool UBEHostMigrationSubsystem::Step2_LeaveLobby()
{
	auto* World{ GetWorld() };
	auto* LobbySubsystem{ World ? UGameInstance::GetSubsystem<UOnlineLobbySubsystem>(World->GetGameInstance()) : nullptr };

	if (ensure(LobbySubsystem))
	{
		auto NewDelegate
		{
			FLobbyLeaveCompleteDelegate::CreateUObject(this, &ThisClass::Step3_LeaveLobbyComplete)
		};

		return LobbySubsystem->CleanUpLobby(NAME_GameSession, World->GetFirstPlayerController(), NewDelegate);
	}

	return false;
}

void UBEHostMigrationSubsystem::Step3_LeaveLobbyComplete(FOnlineServiceResult Result)
{
	if (Result.bWasSuccessful)
	{
		Step4_RecreateLobby();
	}
	else
	{
		HostMigrationException();
	}
}

bool UBEHostMigrationSubsystem::Step4_RecreateLobby()
{
	auto* World{ GetWorld() };
	auto* LobbySubsystem{ World ? UGameInstance::GetSubsystem<UOnlineLobbySubsystem>(World->GetGameInstance()) : nullptr };
	auto* RejoinSubsystem{ World ? UGameInstance::GetSubsystem<UBERejoinMatchSubsystem>(World->GetGameInstance()) : nullptr };

	if (ensure(LobbySubsystem) && (RejoinSubsystem))
	{
		// キャッシュした進行中の対戦ロビーのデータをもとにロビーを再作成する。

		if (const auto* Experience{ RejoinSubsystem->GetOngoingLobbyUserFacingExperience() })
		{
			ULobbyCreateRequest* NewCreateRequest{ nullptr };
			ULobbySearchRequest* NewSearchRequest{ nullptr };

			Experience->CreateRequestsForRejoin(LobbySubsystem, RejoinSubsystem->GetOngoingLobbyUniqueId(), NewCreateRequest, NewSearchRequest);

			if (ensure(NewCreateRequest))
			{
				auto NewDelegate
				{
					FLobbyCreateCompleteDelegate::CreateUObject(this, &ThisClass::Step5_RecreateLobbyComplete)
				};

				return LobbySubsystem->CreateLobby(World->GetFirstPlayerController(), NewCreateRequest, NewDelegate);
			}
		}
	}

	return false;
}

void UBEHostMigrationSubsystem::Step5_RecreateLobbyComplete(ULobbyCreateRequest* Request, FOnlineServiceResult Result)
{
	auto* World{ GetWorld() };

	if (ensure(World))
	{
		if (Result.bWasSuccessful)
		{
			if (Step6_ServerTravel())
			{
				return;
			}
		}

		HostMigrationException();
	}
}

bool UBEHostMigrationSubsystem::Step6_ServerTravel()
{
	auto* World{ GetWorld() };

	if (ensure(World))
	{
		if (ensure(World->ServerTravel(HostMigrationRequestURL, true)))
		{
			ClearHostMigrationRequest();
			return true;
		}
	}

	return false;
}


void UBEHostMigrationSubsystem::HostMigrationException()
{
	ClearHostMigrationRequest();
	UGameplayStatics::OpenLevel(this, NAME_None);
}