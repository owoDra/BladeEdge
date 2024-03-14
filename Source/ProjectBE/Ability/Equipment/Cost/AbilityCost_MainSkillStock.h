// Copyright (C) 2024 owoDra

#pragma once

#include "Ability/Equipment/Cost/AbilityCost_TimeChargeSkillStock.h"

#include "AbilityCost_MainSkillStock.generated.h"


/**
 * メインスキル用の StatTag コスト
 */
UCLASS(DefaultToInstanced, EditInlineNew, meta = (DisplayName = "Main Skill Stock Cost "))
class UAbilityCost_MainSkillStock : public UAbilityCost_TimeChargeSkillStock
{
	GENERATED_BODY()
public:
	UAbilityCost_MainSkillStock(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataBase", meta = (Categories = "DataBase.MainSkill.Stock"))
	FGameplayTag StockTag;

public:
	virtual void OnAvatarSet(
		const UGAEGameplayAbility* Ability
		, const FGameplayAbilityActorInfo* ActorInfo
		, const FGameplayAbilitySpec& Spec) override;

};
