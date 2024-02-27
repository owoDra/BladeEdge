// Copyright (C) 2024 owoDra

#include "BETask_WaitPlayers.h"

#include "Character/BECharacter.h"

#include "Engine/EngineTypes.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BETask_WaitPlayers)


UBETask_WaitPlayers::UBETask_WaitPlayers(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UBETask_WaitPlayers* UBETask_WaitPlayers::TaskWaitPlayers(TScriptInterface<IGameplayTaskOwnerInterface> TaskOwner, int32 InRequiredPlayers, float CheckInterval, int32 Priority)
{
	auto* Task{ NewTaskUninitialized<UBETask_WaitPlayers>() };

	if (Task && TaskOwner.GetInterface())
	{
		Task->InitTask(*TaskOwner, Priority);
		Task->RequiredPlayers = InRequiredPlayers;
		Task->Interval = CheckInterval;
	}

	return Task;
}


void UBETask_WaitPlayers::Activate()
{
	HandleCheck();
}

void UBETask_WaitPlayers::OnDestroy(bool bInOwnerFinished)
{
	if (auto* World{ GetWorld() })
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
	}

	Super::OnDestroy(bInOwnerFinished);
}


void UBETask_WaitPlayers::HandleCheck()
{
	auto* World{ GetWorld() };

	// 全員そろたっか確認

	if (CheckHasAllReady(World))
	{
		HandleAllReady();
		return;
	}

	// インターバールが0以下の場合は次のフレームでチェックをする

	if (Interval <= 0)
	{
		TimerHandle = World->GetTimerManager().SetTimerForNextTick(this, &UBETask_WaitPlayers::HandleCheck);
	}
	else
	{
		World->GetTimerManager().SetTimer(TimerHandle, this, &UBETask_WaitPlayers::HandleCheck, Interval, false);
	}
}

bool UBETask_WaitPlayers::CheckHasAllReady(UWorld* InWorld)
{
	if (auto* GS{ GetGameState(InWorld) })
	{
		int32 NumPlayersReady{ 0 };

		for (const auto& PlayerState : GS->PlayerArray)
		{
			auto* BECharacter{ Cast<ABECharacter>(PlayerState->GetPawn()) };
			if (BECharacter && BECharacter->IsGameReady())
			{
				++NumPlayersReady;
			}
		}

		const auto bChanged{ CurrentPlayers != NumPlayersReady };
		const auto bAllReady{ NumPlayersReady == RequiredPlayers };

		CurrentPlayers = NumPlayersReady;

		if (bChanged)
		{
			HandleChanged();
		}

		return bAllReady;
	}

	return false;
}


void UBETask_WaitPlayers::HandleAllReady()
{
	OnReady.Broadcast(CurrentPlayers, RequiredPlayers);
	EndTask();
}

void UBETask_WaitPlayers::HandleChanged()
{
	OnChanged.Broadcast(CurrentPlayers, RequiredPlayers);
}


AGameStateBase* UBETask_WaitPlayers::GetGameState(UWorld* InWorld) const
{
	if (GameState.IsValid())
	{
		return GameState.Get();
	}

	auto* GameMode{ InWorld->GetAuthGameMode() };
	GameState = GameMode ? GameMode->GetGameState<AGameStateBase>() : nullptr;

	return GameState.Get();
}
