// Copyright (C) 2024 owoDra

#include "BEHostMigrationComponent.h"

#include "ProjectBELogs.h"

#include "GameDelegates.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameStateBase.h"

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
	const auto bIsClient{ !NetDriver->IsServer() };
	if (!bIsClient)
	{
		return;
	}

	UE_LOG(LogBE_HostMigration, Log, TEXT("[%s]Disconeccted"), NetDriver->IsServer() ? TEXT("SERVER") : TEXT("CLIENT"));
	UE_LOG(LogBE_HostMigration, Log, TEXT("| NetDriver: %s"), *GetNameSafe(NetDriver));
	UE_LOG(LogBE_HostMigration, Log, TEXT("| World: %s"), *GetNameSafe(InWorld));
	
	UE_LOG(LogBE_HostMigration, Log, TEXT("| PlayerControlers"));
	for (auto It{ InWorld->GetPlayerControllerIterator() }; It; ++It)
	{
		auto PC{ *It };
		UE_LOG(LogBE_HostMigration, Log, TEXT("| + PC[%d]: %s"), It.GetIndex(), *GetNameSafe(PC.Get()));
	}

	auto* FirstPC{ InWorld->GetFirstPlayerController() };
	UE_LOG(LogBE_HostMigration, Log, TEXT("| FirstPlayerController: %s"), *GetNameSafe(FirstPC));

	auto* FistPS{ FirstPC ? FirstPC->GetPlayerState<APlayerState>() : nullptr };
	UE_LOG(LogBE_HostMigration, Log, TEXT("| FirstPlayerState: %s"), *GetNameSafe(FistPS));

	auto* GM{ InWorld->GetAuthGameMode() };
	UE_LOG(LogBE_HostMigration, Log, TEXT("| AuthGameMode: %s"), *GetNameSafe(GM));

	auto* GS{ InWorld->GetGameState() };
	UE_LOG(LogBE_HostMigration, Log, TEXT("| GameState: %s"), *GetNameSafe(GS));

	UE_LOG(LogBE_HostMigration, Log, TEXT("| PlayerArray"));
	auto Players{ GS ? GS->PlayerArray : TArray<TObjectPtr<APlayerState>>() };

	for (const auto& PlayerState : Players)
	{
		UE_LOG(LogBE_HostMigration, Log, TEXT("| + PS: %s"), *GetNameSafe(PlayerState));

		if (PlayerState)
		{
			UE_LOG(LogBE_HostMigration, Log, TEXT("| ++ PlayerId: %d"), PlayerState->GetPlayerId());
			UE_LOG(LogBE_HostMigration, Log, TEXT("| ++ PlayerName: %s"), *PlayerState->GetPlayerName());
			UE_LOG(LogBE_HostMigration, Log, TEXT("| ++ UniqueId: %s"), *PlayerState->GetUniqueId().ToDebugString());
			UE_LOG(LogBE_HostMigration, Log, TEXT("| ++ IsFirst: %s"), (FistPS == PlayerState) ? TEXT("YES") : TEXT("NO"));
		}
	}
}
