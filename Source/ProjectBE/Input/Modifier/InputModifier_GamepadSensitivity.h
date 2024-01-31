// Copyright (C) 2024 owoDra

#pragma once

#include "InputModifiers.h"

#include "InputModifier_GamepadSensitivity.generated.h"

class UGamepadSensitivityData;


/** 
 * 設定値に基づいてゲームパッド感度を適用させる InputModifier クラス
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "IM Gamepad Sensitivity"))
class UInputModifier_GamepadSensitivity : public UInputModifier
{
	GENERATED_BODY()
public:
	UInputModifier_GamepadSensitivity() {}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensitivity", meta=(AssetBundles="Client, Server"))
	TObjectPtr<const UGamepadSensitivityData> SensitivityLevelTable;

protected:
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;

};
