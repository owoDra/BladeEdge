// Copyright (C) 2024 owoDra

#include "BEGameplayAbility_EquipmentBase.h"

#include "GameplayTag/BETags_Message.h"
#include "GameplayTag/BETags_Flag.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEGameplayAbility_EquipmentBase)


UBEGameplayAbility_EquipmentBase::UBEGameplayAbility_EquipmentBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	ActivationMethod = EAbilityActivationMethod::OnInputTriggered;
	ActivationPolicy = EAbilityActivationPolicy::Default;

	ActivationMessageTag = FGameplayTag::EmptyTag;
	bActivationMessageLocallyOnly = true;

	bUseCooldown = true;
	CooltimeOverride = 2.0f;
	CooldownMessageTag = FGameplayTag::EmptyTag;

	ActivationBlockedTags.AddTag(TAG_Flag_EquipmentUseBlocked);
}
