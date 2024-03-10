// Copyright (C) 2024 owoDra

#include "BEHostMigrationSubsystem.h"

#include "ProjectBELogs.h"

// Game Online Core
#include "OnlineLobbySubsystem.h"
#include "Type/OnlineLobbyCreateTypes.h"

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
		auto* World{ GetWorld() };

		if (ensure(World))
		{
			if (auto* LobbySubsystem{ UGameInstance::GetSubsystem<UOnlineLobbySubsystem>(World->GetGameInstance()) })
			{
				if (LobbySubsystem->GetJoinedLobby(NAME_GameSession))
				{
					if (ensure(World->ServerTravel(HostMigrationRequestURL, true)))
					{
						ClearHostMigrationRequest();
						return true;
					}
				}
			}
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