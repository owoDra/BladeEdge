// Copyright (C) 2024 owoDra

#pragma once

#include "Ability/Equipment/BEGameplayAbility_EquipmentBase.h"

#include "BEGameplayAbility_SubSkill.generated.h"


/**
 * サブアビリティのためのベースクラス
 */
UCLASS(Abstract)
class PROJECTBE_API UBEGameplayAbility_SubSkill : public UBEGameplayAbility_EquipmentBase
{
	GENERATED_BODY()
public:
	UBEGameplayAbility_SubSkill(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataBase", meta = (Categories = "DataBase.SubSkill.Cooltime"))
	FGameplayTag CooltimeTag;

};
