// Copyright (C) 2024 owoDra

#include "BETask_ObservePlayerCount.h"

#include "Engine/EngineTypes.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BETask_ObservePlayerCount)


UBETask_ObservePlayerCount::UBETask_ObservePlayerCount(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UBETask_ObservePlayerCount* UBETask_ObservePlayerCount::TaskObservePlayerCount(TScriptInterface<IGameplayTaskOwnerInterface> TaskOwner, int32 InRequiredPlayers, float CheckInterval, bool bDefaultEnough, int32 Priority)
{
	auto* Task{ NewTaskUninitialized<UBETask_ObservePlayerCount>() };

	if (Task && TaskOwner.GetInterface())
	{
		Task->InitTask(*TaskOwner, Priority);
		Task->RequiredPlayers = InRequiredPlayers;
		Task->bEnough = bDefaultEnough;
		Task->Interval = FMath::Max(CheckInterval, 0.1f);
	}

	return Task;
}


void UBETask_ObservePlayerCount::Activate()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UBETask_ObservePlayerCount::HandleCheck, Interval, true);

	HandleCheck();
}

void UBETask_ObservePlayerCount::OnDestroy(bool bInOwnerFinished)
{
	if (auto* World{ GetWorld() })
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
	}

	Super::OnDestroy(bInOwnerFinished);
}


void UBETask_ObservePlayerCount::HandleCheck()
{
	const auto bNewEnough{ GetNumPlayers() >= RequiredPlayers };

	if (bNewEnough != bEnough)
	{
		bEnough = bNewEnough;

		HandleChanged();
	}
}

void UBETask_ObservePlayerCount::HandleChanged()
{
	OnChanged.Broadcast(bEnough);
}

int32 UBETask_ObservePlayerCount::GetNumPlayers() const
{
	if (GameMode.IsValid())
	{
		return GameMode->GetNumPlayers();
	}

	const auto* World{ GetWorld() };
	GameMode = World ? World->GetAuthGameMode() : nullptr;

	if (GameMode.IsValid())
	{
		return GameMode->GetNumPlayers();
	}

	return int32();
}
