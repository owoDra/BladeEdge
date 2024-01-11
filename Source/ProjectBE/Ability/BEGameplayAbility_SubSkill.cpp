﻿// Copyright (C) 2024 owoDra

#include "BEGameplayAbility_SubSkill.h"

#include "GameplayTag/BETags_Ability.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEGameplayAbility_SubSkill)


UBEGameplayAbility_SubSkill::UBEGameplayAbility_SubSkill(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CooldownTag = TAG_Ability_Cooldown_Skill_Sub;
}