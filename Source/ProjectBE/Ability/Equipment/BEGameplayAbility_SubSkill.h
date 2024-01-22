// Copyright (C) 2024 owoDra

#pragma once

#include "BEGameplayAbility_MainSkill.h"

#include "BEGameplayAbility_SubSkill.generated.h"


/**
 * サブアビリティのためのベースクラス
 */
UCLASS(Abstract)
class PROJECTBE_API UBEGameplayAbility_SubSkill : public UBEGameplayAbility_MainSkill
{
	GENERATED_BODY()
public:
	UBEGameplayAbility_SubSkill(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
