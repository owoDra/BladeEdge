// Copyright (C) 2023 owoDra

#pragma once

#include "InputModifiers.h"

#include "InputModifier_GamepadSensitivity.generated.h"

class UGamepadSensitivityData;


/** 
 * 適用させる感度のターゲティングタイプ
 */
UENUM()
enum class EGamepadTargetingType : uint8
{
	// 通常時
	Normal = 0,

	// ターゲティング時
	Targeting = 1,
};


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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensitivity")
	EGamepadTargetingType TargetingType{ EGamepadTargetingType::Normal };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensitivity", meta=(AssetBundles="Client,Server"))
	TObjectPtr<const UGamepadSensitivityData> SensitivityLevelTable;

protected:
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;

};
