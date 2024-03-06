// Copyright (C) 2024 owoDra

#pragma once

#include "Execution/HealthExecutionModifier.h"

#include "HealthExecutionModifier_AddUltPoint.generated.h"

struct FGameplayEffectCustomExecutionParameters;


/**
 * ダメージに応じて加害者にウルトポイントを付与するモディファイヤー
 */
UCLASS()
class UHealthExecutionModifier_AddUltPoint : public UHealthExecutionModifier
{
	GENERATED_BODY()
public:
	UHealthExecutionModifier_AddUltPoint(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual float ModifierExecution(float Base, const FGameplayEffectCustomExecutionParameters& ExecutionParams) const override;

};
