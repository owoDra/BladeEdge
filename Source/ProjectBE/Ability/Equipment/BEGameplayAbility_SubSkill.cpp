// Copyright (C) 2024 owoDra

#include "BEGameplayAbility_SubSkill.h"

#include "GameplayTag/BETags_Input.h"
#include "GameplayTag/BETags_Message.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEGameplayAbility_SubSkill)


UBEGameplayAbility_SubSkill::UBEGameplayAbility_SubSkill(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ActivationMessageTag = TAG_Message_Ability_Activation_SubSkill;
	CooldownMessageTag = TAG_Message_Ability_Cooldown_SubSkill;

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	TriggerData.TriggerTag = TAG_Input_Shared_SubSkill;
	AbilityTriggers.Empty(1);
	AbilityTriggers.Add(TriggerData);
}
