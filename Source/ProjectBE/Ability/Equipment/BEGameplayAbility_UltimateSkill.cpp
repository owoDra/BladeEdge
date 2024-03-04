// Copyright (C) 2024 owoDra

#include "BEGameplayAbility_UltimateSkill.h"

#include "GameplayTag/BETags_Message.h"
#include "GameplayTag/BETags_Stat.h"
#include "GameplayTag/BETags_Input.h"

// Game Ability Extension
#include "Cost/AbilityCost_StatTag.h"
#include "GameplayTag/GAETags_Ability.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEGameplayAbility_UltimateSkill)


UBEGameplayAbility_UltimateSkill::UBEGameplayAbility_UltimateSkill(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ActivationMessageTag = TAG_Message_Ability_Activation_UltimateSkill;
	CooldownMessageTag = TAG_Message_Ability_Cooldown_UltimateSkill;

	auto* NewCost{ ObjectInitializer.CreateDefaultSubobject<UAbilityCost_StatTag>(this, TEXT("PointCost")) };
	auto& NewCostDef{ NewCost->StatTagCosts.AddDefaulted_GetRef() };
	NewCostDef.StatTag = TAG_Stat_UltimatePoint;
	NewCostDef.Target = EStatTagCostTarget::PlayerState;
	NewCostDef.Cost = 100.0f;
	NewCostDef.bShouldInitStatTag = true;
	NewCostDef.bDoNotInitDefaultValue = true;
	NewCostDef.MaxValue = 100.0f;
	NewCostDef.DefaultValue = 0.0f;
	AdditionalCosts.Add(NewCost);

	AbilityTags.AddTag(TAG_Ability_Type_Skill);

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	TriggerData.TriggerTag = TAG_Input_MouseAndKeyboard_UltimateSkill;
	AbilityTriggers.Empty(1);
	AbilityTriggers.Add(TriggerData);
}
