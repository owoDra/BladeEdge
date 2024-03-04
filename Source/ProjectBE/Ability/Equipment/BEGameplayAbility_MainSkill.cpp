// Copyright (C) 2024 owoDra

#include "BEGameplayAbility_MainSkill.h"

#include "GameplayTag/BETags_Input.h"
#include "GameplayTag/BETags_Message.h"
#include "Ability/Equipment/Cost/AbilityCost_TimeChargeSkillStock.h"

// Game Ability Extension
#include "GameplayTag/GAETags_Ability.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEGameplayAbility_MainSkill)


UBEGameplayAbility_MainSkill::UBEGameplayAbility_MainSkill(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ActivationPolicy = EAbilityActivationPolicy::CostOverCooldown;

	ActivationMessageTag = TAG_Message_Ability_Activation_MainSkill;
	CooldownMessageTag = TAG_Message_Ability_Cooldown_MainSkill;

	auto* NewCost{ ObjectInitializer.CreateDefaultSubobject<UAbilityCost_TimeChargeSkillStock>(this, TEXT("StockCost"))};
	AdditionalCosts.Add(NewCost);

	AbilityTags.AddTag(TAG_Ability_Type_Skill);

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	TriggerData.TriggerTag = TAG_Input_Shared_MainSkill;
	AbilityTriggers.Empty(1);
	AbilityTriggers.Add(TriggerData);
}
