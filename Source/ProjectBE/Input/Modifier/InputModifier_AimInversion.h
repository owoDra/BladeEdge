// Copyright (C) 2024 owoDra

#pragma once

#include "InputModifiers.h"

#include "InputModifier_AimInversion.generated.h"


/** 
 * ユーザー設定の基づいて入力の値を反転させる InputModifier クラス
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "IM Aim Inversion Setting"))
class UInputModifier_AimInversion : public UInputModifier
{
	GENERATED_BODY()
public:
	UInputModifier_AimInversion() {}
	
protected:
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;	

};
