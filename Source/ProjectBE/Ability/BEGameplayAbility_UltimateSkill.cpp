// Copyright (C) 2024 owoDra

#include "BEGameplayAbility_UltimateSkill.h"

#include "GameplayTag/BETags_Ability.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEGameplayAbility_UltimateSkill)


UBEGameplayAbility_UltimateSkill::UBEGameplayAbility_UltimateSkill(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CooldownTag = TAG_Ability_Cooldown_Skill_Ultimate;
}
