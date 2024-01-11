// Copyright (C) 2024 owoDra

#pragma once

#include "Ability/GameplayAbility_TimeChargeSkill.h"

#include "BEGameplayAbility_MainSkill.generated.h"


/**
 * メインアビリティのためのベースクラス
 */
UCLASS(Abstract)
class PROJECTBE_API UBEGameplayAbility_MainSkill : public UGameplayAbility_TimeChargeSkill
{
	GENERATED_BODY()
public:
	UBEGameplayAbility_MainSkill(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
