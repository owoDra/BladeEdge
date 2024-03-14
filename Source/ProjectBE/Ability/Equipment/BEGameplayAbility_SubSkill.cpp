﻿// Copyright (C) 2024 owoDra

#include "BEGameplayAbility_SubSkill.h"

#include "GameplayTag/BETags_Input.h"
#include "GameplayTag/BETags_Message.h"
#include "GameplayTag/BEDataBaseTags.h"
#include "BEDataBaseSubsystem.h"
#include "Ability/Equipment/Cost/AbilityCost_SubSkillStock.h"

// Game Item Core
#include "ItemData.h"

// Game Equipment Extension
#include "Equipment/Equipment.h"

// Game Ability Extension
#include "GameplayTag/GAETags_Ability.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEGameplayAbility_SubSkill)


UBEGameplayAbility_SubSkill::UBEGameplayAbility_SubSkill(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ActivationPolicy = EAbilityActivationPolicy::CostOverCooldown;

	ActivationMessageTag = TAG_Message_Ability_Activation_SubSkill;
	CooldownMessageTag = TAG_Message_Ability_Cooldown_SubSkill;

	auto* NewCost{ ObjectInitializer.CreateDefaultSubobject<UAbilityCost_SubSkillStock>(this, TEXT("StockCost")) };
	AdditionalCosts.Add(NewCost);

	AbilityTags.AddTag(TAG_Ability_Type_Skill);

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	TriggerData.TriggerTag = TAG_Input_Shared_SubSkill;
	AbilityTriggers.Empty(1);
	AbilityTriggers.Add(TriggerData);

	CooltimeTag = TAG_DataBase_SubSkill_Cooltime;
}


void UBEGameplayAbility_SubSkill::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	const auto* Avatar{ ActorInfo->AvatarActor.Get() };
	const auto* World{ Avatar ? Avatar->GetWorld() : nullptr };
	const auto* DataBase{ World ? UGameInstance::GetSubsystem<UBEDataBaseSubsystem>(World->GetGameInstance()) : nullptr };

	if (ensure(DataBase))
	{
		CooltimeOverride = DataBase->GetEquipmentParameter(DataBaseKey, CooltimeTag, CooltimeOverride);
	}

	Super::OnAvatarSet(ActorInfo, Spec);
}
