// Copyright (C) 2024 owoDra

#pragma once

#include "Cost/AbilityCost_StatTag.h"

#include "AbilityCost_UltimateSkillPoint.generated.h"


/**
 * ウルトスキル用の StatTag コスト
 */
UCLASS(DefaultToInstanced, EditInlineNew, meta = (DisplayName = "Ult Skill Stock Cost "))
class UAbilityCost_UltimateSkillPoint : public UAbilityCost_StatTag
{
	GENERATED_BODY()
public:
	UAbilityCost_UltimateSkillPoint(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataBase", meta = (Categories = "DataBase.UltSkill.Cost"))
	FGameplayTag CostTag;

public:
	virtual void OnAvatarSet(
		const UGAEGameplayAbility* Ability
		, const FGameplayAbilityActorInfo* ActorInfo
		, const FGameplayAbilitySpec& Spec) override;

};
