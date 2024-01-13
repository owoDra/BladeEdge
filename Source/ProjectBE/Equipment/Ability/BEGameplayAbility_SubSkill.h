// Copyright (C) 2024 owoDra

#pragma once

#include "Ability/GameplayAbility_TimeChargeSkill.h"

#include "BEGameplayAbility_SubSkill.generated.h"


/**
 * サブアビリティのためのベースクラス
 */
UCLASS(Abstract)
class PROJECTBE_API UBEGameplayAbility_SubSkill : public UGameplayAbility_TimeChargeSkill
{
	GENERATED_BODY()
public:
	UBEGameplayAbility_SubSkill(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
