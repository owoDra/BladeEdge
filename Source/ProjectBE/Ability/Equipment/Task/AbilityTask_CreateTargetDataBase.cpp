// Copyright (C) 2024 owoDra

#include "AbilityTask_CreateTargetDataBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AbilityTask_CreateTargetDataBase)


UAbilityTask_CreateTargetDataBase::UAbilityTask_CreateTargetDataBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


void UAbilityTask_CreateTargetDataBase::HandleTargetDataCreated(const FGameplayAbilityTargetDataHandle& InTargetData, bool bShouldFinishTask)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnCreated.Broadcast(InTargetData);
	}

	if (bShouldFinishTask)
	{
		EndTask();
	}
}

void UAbilityTask_CreateTargetDataBase::HandleTargetDataNotCreated()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnNotCreated.Broadcast();
	}

	EndTask();
}
