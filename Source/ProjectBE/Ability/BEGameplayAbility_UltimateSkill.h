// Copyright (C) 2024 owoDra

#pragma once

#include "Ability/GameplayAbility_PointChargeSkill.h"

#include "BEGameplayAbility_UltimateSkill.generated.h"


/**
 * アルティメットアビリティのためのベースクラス
 */
UCLASS(Abstract)
class PROJECTBE_API UBEGameplayAbility_UltimateSkill : public UGameplayAbility_PointChargeSkill
{
	GENERATED_BODY()
public:
	UBEGameplayAbility_UltimateSkill(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
