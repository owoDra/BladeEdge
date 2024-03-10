// Copyright (C) 2024 owoDra

#include "BEHostMigrationComponent.h"

#include "HostMigration/BEHostMigrationSubsystem.h"
#include "GameMode/BEMatchTimerComponent.h"
#include "ProjectBELogs.h"

// Game Online Core
#include "OnlineLobbySubsystem.h"
#include "Type/OnlineLobbyResultTypes.h"

// Game Phase Extension
#include "GamePhaseSubsystem.h"

// Game Team Extension
#include "TeamManagerSubsystem.h"

// Engine Features
#include "GameDelegates.h"
#include "Online/OnlineSessionNames.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEHostMigrationComponent)

////////////////////////////////////////////////////////////////////////////

UBEHostMigrationComponent::UBEHostMigrationComponent(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(false);
	bAutoRegister = true;
	bAutoActivate = true;
	bWantsInitializeComponent = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}


// Initialization

void UBEHostMigrationComponent::InitializeComponent()
{
	Super::InitializeComponent();

	DisconnectHandle = FGameDelegates::Get().GetHandleDisconnectDelegate().AddUObject(this, &ThisClass::HandleDisconnect);
}

void UBEHostMigrationComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	FGameDelegates::Get().GetHandleDisconnectDelegate().Remove(DisconnectHandle);

	Super::EndPlay(EndPlayReason);
}

void UBEHostMigrationComponent::HandleDisconnect(UWorld* InWorld, UNetDriver* NetDriver)
{
	// 切断したのがホストだった場合はマイグレーションを行わない

	const auto bIsClient{ !NetDriver->IsServer() };
	if (!bIsClient)
	{
		return;
	}

	UE_LOG(LogBE_HostMigration, Log, TEXT("[%s]Disconeccted"), NetDriver->IsServer() ? TEXT("SERVER") : TEXT("CLIENT"));
	UE_LOG(LogBE_HostMigration, Log, TEXT("| NetDriver: %s"), *GetNameSafe(NetDriver));
	UE_LOG(LogBE_HostMigration, Log, TEXT("| World: %s"), *GetNameSafe(InWorld));

	const auto NewURL{ CreateNewURL(InWorld, NetDriver) };

	SendHostMigrationRequest(InWorld, NewURL);
}


FString UBEHostMigrationComponent::CreateNewURL(UWorld* InWorld, UNetDriver* NetDriver) const
{
	FString Map;
	FString Options{ TEXT("?listen") };

	auto* GameInstance{ InWorld->GetGameInstance() };

	// ロビーから情報を取得
	// - マップ名を設定
	// - ExperienceDataを指定

	auto* LobbySubsystem{ UGameInstance::GetSubsystem<UOnlineLobbySubsystem>(GameInstance) };
	if (ensure(LobbySubsystem))
	{
		if (auto* CurrentLobby{ LobbySubsystem->GetJoinedLobby(NAME_GameSession) })
		{
			CurrentLobby->GetLobbyAttributeAsString(SETTING_MAPNAME, Map);

			FString ExperienceData;
			CurrentLobby->GetLobbyAttributeAsString(SETTING_GAMEMODE, ExperienceData);
			Options += FString::Printf(TEXT("?ExperienceData=%s"), *ExperienceData);
		}
	}

	// GamePhase から情報を取得
	// - 開始時 GamePhase を設定

	auto* GamePhaseSubsystem{ UWorld::GetSubsystem<UGamePhaseSubsystem>(InWorld) };
	if (ensure(GamePhaseSubsystem))
	{
		Options += GamePhaseSubsystem->ConstructGameModeOption();
	}

	// TeamManager から情報を取得
	// - 開始時 TeamStat を設定
	// - 開始時 Team分け を設定

	auto* TeamManagerSubsystem{ UWorld::GetSubsystem<UTeamManagerSubsystem>(InWorld) };
	if (ensure(TeamManagerSubsystem))
	{
		Options += TeamManagerSubsystem->ConstructGameModeOption();
	}

	// MatchTimer から情報を取得
	// - 開始時タイマー情報を設定

	auto* GameState{ InWorld->GetGameState() };
	if (auto* MatchTimer{ GameState->FindComponentByClass<UBEMatchTimerComponent>() })
	{
		Options += MatchTimer->ConstructGameModeOption();
	}

	return FString::Printf(TEXT("%s%s"), *Map, *Options);
}

void UBEHostMigrationComponent::SendHostMigrationRequest(UWorld* InWorld, const FString& NewURL)
{
	if (bShouldHostMigration)
	{
		auto* GameInstance{ InWorld->GetGameInstance() };
		auto* HostMigrationSubsystem{ UGameInstance::GetSubsystem<UBEHostMigrationSubsystem>(GameInstance) };
		if (ensure(HostMigrationSubsystem))
		{
			HostMigrationSubsystem->HandleHostMigrationRequestURL(NewURL);
		}
	}
}
