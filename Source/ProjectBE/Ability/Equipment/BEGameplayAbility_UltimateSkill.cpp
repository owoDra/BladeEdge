// Copyright (C) 2024 owoDra

#include "BEGameplayAbility_UltimateSkill.h"

#include "GameplayTag/BETags_Message.h"
#include "GameplayTag/BETags_Input.h"
#include "Ability/Equipment/Cost/AbilityCost_UltimateSkillPoint.h"

// Game Ability Extension
#include "GameplayTag/GAETags_Ability.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEGameplayAbility_UltimateSkill)


UBEGameplayAbility_UltimateSkill::UBEGameplayAbility_UltimateSkill(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ActivationMessageTag = TAG_Message_Ability_Activation_UltimateSkill;
	CooldownMessageTag = TAG_Message_Ability_Cooldown_UltimateSkill;

	auto* NewCost{ ObjectInitializer.CreateDefaultSubobject<UAbilityCost_UltimateSkillPoint>(this, TEXT("PointCost")) };
	AdditionalCosts.Add(NewCost);

	AbilityTags.AddTag(TAG_Ability_Type_Skill);

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	TriggerData.TriggerTag = TAG_Input_MouseAndKeyboard_UltimateSkill;
	AbilityTriggers.Empty(1);
	AbilityTriggers.Add(TriggerData);
}
