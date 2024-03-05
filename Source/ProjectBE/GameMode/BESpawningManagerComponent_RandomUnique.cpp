// Copyright (C) 2024 owoDra

#include "BESpawningManagerComponent_RandomUnique.h"

#include "EngineUtils.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/GameModeBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BESpawningManagerComponent_RandomUnique)

////////////////////////////////////////////////////////////////////////////

UBESpawningManagerComponent_RandomUnique::UBESpawningManagerComponent_RandomUnique(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

AActor* UBESpawningManagerComponent_RandomUnique::OnChoosePlayerStart(AController* Player, TArray<APlayerStart*>& PlayerStarts)
{
	TArray<APlayerStart*> ValidStarts;
	for (const auto& Start : PlayerStarts)
	{
		if (!ChosenPlayerStarts.Contains(Start))
		{
			ValidStarts.Add(Start);
		}
	}

	if (auto* ValidStart{ GetFirstRandomUnoccupiedPlayerStart(Player, ValidStarts) })
	{
		ChosenPlayerStarts.Add(ValidStart);
		SetChosenListResetTimer();
		return ValidStart;
	}

	return nullptr;
}

void UBESpawningManagerComponent_RandomUnique::SetChosenListResetTimer()
{
	if (auto* World{ GetWorld() })
	{
		auto& Timer{ World->GetTimerManager() };

		if (ChosenListResetTimerHandle.IsValid())
		{
			Timer.ClearTimer(ChosenListResetTimerHandle);
		}

		Timer.SetTimer(ChosenListResetTimerHandle, this, &ThisClass::HandleChosenListReset, ChosenListResetTimerInterval, false);
	}
}

void UBESpawningManagerComponent_RandomUnique::HandleChosenListReset()
{
	ChosenPlayerStarts.Reset();
}
