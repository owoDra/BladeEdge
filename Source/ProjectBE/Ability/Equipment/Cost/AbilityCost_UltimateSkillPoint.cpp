// Copyright (C) 2024 owoDra

#include "AbilityCost_UltimateSkillPoint.h"

#include "GameplayTag/BETags_Stat.h"
#include "GameplayTag/BEDataBaseTags.h"
#include "BEDataBaseSubsystem.h"
#include "Ability/Equipment/BEGameplayAbility_EquipmentBase.h"

// Game Item Core
#include "ItemData.h"

// Game Equipment Extension
#include "Equipment/Equipment.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AbilityCost_UltimateSkillPoint)


UAbilityCost_UltimateSkillPoint::UAbilityCost_UltimateSkillPoint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	auto& NewCostDef{ StatTagCosts.AddDefaulted_GetRef() };
	NewCostDef.StatTag = TAG_Stat_UltimatePoint;
	NewCostDef.Target = EStatTagCostTarget::PlayerState;
	NewCostDef.Cost = 100.0f;
	NewCostDef.bShouldInitStatTag = true;
	NewCostDef.bDoNotInitDefaultValue = true;
	NewCostDef.MaxValue = 100.0f;
	NewCostDef.DefaultValue = 0.0f;

	CostTag = TAG_DataBase_UltSkill_Cost;
}


void UAbilityCost_UltimateSkillPoint::OnAvatarSet(const UGAEGameplayAbility* Ability, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
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
			Cost.Cost			= DataBase->GetEquipmentParameter(Key, CostTag, Cost.Cost.GetValue());
			Cost.MaxValue		= DataBase->GetEquipmentParameter(Key, CostTag, Cost.MaxValue.GetValue());
		}
	}

	Super::OnAvatarSet(Ability, ActorInfo, Spec);
}
