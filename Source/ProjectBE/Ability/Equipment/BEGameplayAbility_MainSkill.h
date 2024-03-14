// Copyright (C) 2024 owoDra

#pragma once

#include "Ability/Equipment/BEGameplayAbility_EquipmentBase.h"

#include "BEGameplayAbility_MainSkill.generated.h"


/**
 * メインアビリティのためのベースクラス
 */
UCLASS(Abstract)
class PROJECTBE_API UBEGameplayAbility_MainSkill : public UBEGameplayAbility_EquipmentBase
{
	GENERATED_BODY()
public:
	UBEGameplayAbility_MainSkill(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataBase", meta = (Categories = "DataBase.MainSkill.Cooltime"))
	FGameplayTag CooltimeTag;

};
