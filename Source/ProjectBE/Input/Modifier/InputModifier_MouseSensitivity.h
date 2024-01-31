// Copyright (C) 2024 owoDra

#pragma once

#include "InputModifiers.h"

#include "InputModifier_MouseSensitivity.generated.h"


/** 
 * 設定の値を入力値に反映させる InputModifer クラス
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "IM Mouse Sensitivity"))
class UInputModifier_MouseSensitivity : public UInputModifier
{
	GENERATED_BODY()
public:
	UInputModifier_MouseSensitivity() {}

protected:
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;

};
