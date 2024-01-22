// Copyright (C) 2024 owoDra

#pragma once

#include "Ability/Equipment/BEGameplayAbility_EquipmentBase.h"

#include "BEGameplayAbility_UltimateSkill.generated.h"


/**
 * アルティメットアビリティのためのベースクラス
 */
UCLASS(Abstract)
class PROJECTBE_API UBEGameplayAbility_UltimateSkill : public UBEGameplayAbility_EquipmentBase
{
	GENERATED_BODY()
public:
	UBEGameplayAbility_UltimateSkill(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
