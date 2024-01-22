// Copyright (C) 2024 owoDra

#pragma once

#include "GAEGameplayAbility.h"

#include "BEGameplayAbility_EquipmentBase.generated.h"


/**
 * このプロジェクトの装備用アビリティのベースクラス
 */
UCLASS(Abstract)
class PROJECTBE_API UBEGameplayAbility_EquipmentBase : public UGAEGameplayAbility
{
	GENERATED_BODY()
public:
	UBEGameplayAbility_EquipmentBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
