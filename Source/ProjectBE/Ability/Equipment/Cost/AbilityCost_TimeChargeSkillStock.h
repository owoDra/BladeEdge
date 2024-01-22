// Copyright (C) 2024 owoDra

#pragma once

#include "Cost/AbilityCost_StatTag.h"

#include "AbilityCost_TimeChargeSkillStock.generated.h"


/**
 * クールダウン時にストックが回復するタイプのストックコスト
 */
UCLASS(DefaultToInstanced, EditInlineNew, meta = (DisplayName = "Time Charge Skill Stock Cost "))
class UAbilityCost_TimeChargeSkillStock : public UAbilityCost_StatTag
{
	GENERATED_BODY()
public:
	UAbilityCost_TimeChargeSkillStock(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//
	// 一度のチャージで付与されるストック数
	//
	UPROPERTY(AdvancedDisplay, BlueprintReadOnly, EditAnywhere, Category = "Costs")
	FScalableFloat NumToCharge{ 1.0f };

public:
	virtual void OnCooldownEnd(
		const UGAEGameplayAbility* Ability
		, const FGameplayAbilitySpecHandle Handle
		, const FGameplayAbilityActorInfo* ActorInfo
		, const FGameplayAbilityActivationInfo ActivationInfo);

};
