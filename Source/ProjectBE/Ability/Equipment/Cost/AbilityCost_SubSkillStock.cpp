// Copyright (C) 2024 owoDra

#include "AbilityCost_SubSkillStock.h"

#include "GameplayTag/BEDataBaseTags.h"
#include "BEDataBaseSubsystem.h"
#include "Ability/Equipment/BEGameplayAbility_EquipmentBase.h"

// Game Item Core
#include "ItemData.h"

// Game Equipment Extension
#include "Equipment/Equipment.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AbilityCost_SubSkillStock)


UAbilityCost_SubSkillStock::UAbilityCost_SubSkillStock(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	StockTag = TAG_DataBase_SubSkill_Stock;
}


void UAbilityCost_SubSkillStock::OnAvatarSet(const UGAEGameplayAbility* Ability, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	const auto* Avatar{ ActorInfo->AvatarActor.Get() };
	const auto* World{ Avatar ? Avatar->GetWorld() : nullptr };
	const auto* DataBase{ World ? UGameInstance::GetSubsystem<UBEDataBaseSubsystem>(World->GetGameInstance()) : nullptr };

	const auto* Equipment{ Cast<UBEGameplayAbility_EquipmentBase>(Ability) };

	if (ensure(DataBase) && ensure(Equipment))
	{
		const auto Key{ Equipment->DataBaseKey };

		for (auto& Cost : StatTagCosts)
		{
			Cost.MaxValue		= DataBase->GetEquipmentParameter(Key, StockTag, Cost.MaxValue.GetValue());
			Cost.DefaultValue	= DataBase->GetEquipmentParameter(Key, StockTag, Cost.DefaultValue.GetValue());
		}
	}

	Super::OnAvatarSet(Ability, ActorInfo, Spec);
}
