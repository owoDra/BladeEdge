// Copyright (C) 2024 owoDra

#include "AbilityCost_TimeChargeSkillStock.h"

#include "GameplayTag/BETags_Stat.h"

#include "GAEGameplayAbility.h"

#include "GameplayTag/GameplayTagStackInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AbilityCost_TimeChargeSkillStock)


UAbilityCost_TimeChargeSkillStock::UAbilityCost_TimeChargeSkillStock(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	auto& Cost{ StatTagCosts.AddDefaulted_GetRef() };
	Cost.StatTag = TAG_Stat_SkillStock;
	Cost.Target = EStatTagCostTarget::SourceObject;
	Cost.Cost = 1.0f;
	Cost.MaxValue = 3.0f;
	Cost.DefaultValue = 3.0f;
	Cost.bShouldInitStatTag = true;
}


void UAbilityCost_TimeChargeSkillStock::OnCooldownEnd(const UGAEGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (!ActorInfo->IsNetAuthority())
	{
		return;
	}

	const auto AbilityLevel{ Ability->GetAbilityLevel(Handle, ActorInfo) };

	auto bShouldContinueCooldown{ false };

	for (const auto& Cost : StatTagCosts)
	{
		if (auto* Interface{ GetCostTarget<IGameplayTagStackInterface>(Cost.Target) })
		{
			const auto MaxValue{ Interface->GetMaxStatTagStackCount(Cost.StatTag) };

			const auto ChargeReal{ NumToCharge.GetValueAtLevel(AbilityLevel) };
			const auto ChargeValue{ FMath::TruncToInt(ChargeReal) };
			
			const auto CurrentValue{ Interface->AddStatTagStack(Cost.StatTag, ChargeValue) };

			bShouldContinueCooldown |= (MaxValue > CurrentValue);
		}
	}

	if (bShouldContinueCooldown)
	{
		auto* OwnerAbility{ GetOwnerAbility() };

		OwnerAbility->CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, false);
	}
}
