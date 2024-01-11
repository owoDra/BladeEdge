// Copyright (C) 2024 owoDra

#include "BEGameplayAbility_MainSkill.h"

#include "GameplayTag/BETags_Ability.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEGameplayAbility_MainSkill)


UBEGameplayAbility_MainSkill::UBEGameplayAbility_MainSkill(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CooldownTag = TAG_Ability_Cooldown_Skill_Main;
}
