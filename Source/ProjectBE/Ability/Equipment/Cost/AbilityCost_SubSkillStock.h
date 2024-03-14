// Copyright (C) 2024 owoDra

#pragma once

#include "Ability/Equipment/Cost/AbilityCost_TimeChargeSkillStock.h"

#include "AbilityCost_SubSkillStock.generated.h"


/**
 * サブスキル用の StatTag コスト
 */
UCLASS(DefaultToInstanced, EditInlineNew, meta = (DisplayName = "Sub Skill Stock Cost "))
class UAbilityCost_SubSkillStock : public UAbilityCost_TimeChargeSkillStock
{
	GENERATED_BODY()
public:
	UAbilityCost_SubSkillStock(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataBase", meta = (Categories = "DataBase.SubSkill.Stock"))
	FGameplayTag StockTag;

public:
	virtual void OnAvatarSet(
		const UGAEGameplayAbility* Ability
		, const FGameplayAbilityActorInfo* ActorInfo
		, const FGameplayAbilitySpec& Spec) override;

};
