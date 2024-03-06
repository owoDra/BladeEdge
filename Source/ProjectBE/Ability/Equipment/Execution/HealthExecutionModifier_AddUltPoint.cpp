// Copyright (C) 2024 owoDra

#include "HealthExecutionModifier_AddUltPoint.h"

#include "GameplayTag/BETags_Stat.h"

#include "GameplayTag/GameplayTagStackInterface.h"

#include "GameplayEffectTypes.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GameFramework/Pawn.h"
#include "GameFramework//PlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HealthExecutionModifier_AddUltPoint)


UHealthExecutionModifier_AddUltPoint::UHealthExecutionModifier_AddUltPoint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


float UHealthExecutionModifier_AddUltPoint::ModifierExecution(float Base, const FGameplayEffectCustomExecutionParameters& ExecutionParams) const
{
#if WITH_SERVER_CODE

	auto* Causer{ ExecutionParams.GetOwningSpec().GetEffectContext().GetEffectCauser() };
	if (auto* CauserPawn{ Cast<APawn>(Causer) })
	{
		if (auto* CauserPlayerStateTagInterface{ Cast<IGameplayTagStackInterface>(CauserPawn->GetPlayerState()) })
		{
			CauserPlayerStateTagInterface->AddStatTagStack(TAG_Stat_UltimatePoint, FMath::TruncToInt32(Base));
		}
	}

	return Base;

#else
	return Base;
#endif	// #if WITH_SERVER_CODE
}
