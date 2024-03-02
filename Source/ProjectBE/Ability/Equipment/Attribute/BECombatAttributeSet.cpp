// Copyright (C) 2024 owoDra

#include "BECombatAttributeSet.h"

#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BECombatAttributeSet)


UBECombatAttributeSet::UBECombatAttributeSet()
	: AttackSpeed(1.0f)
{
}

void UBECombatAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UBECombatAttributeSet, AttackSpeed, COND_None, REPNOTIFY_Always);
}


void UBECombatAttributeSet::OnRep_AttackSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBECombatAttributeSet, AttackSpeed, OldValue);
}
