// Copyright (C) 2024 owoDra

#pragma once

#include "InputModifiers.h"

#include "InputModifier_MouseSensitivity.generated.h"


/**
 * 適用させる感度のターゲティングタイプ
 */
UENUM()
enum class EMouseTargetingType : uint8
{
	// 通常時
	Normal = 0,

	// ターゲティング時
	Targeting = 1,
};


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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensitivity")
	EMouseTargetingType TargetingType{ EMouseTargetingType::Normal };

protected:
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;

};
