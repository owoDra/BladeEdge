// Copyright (C) 2024 owoDra

#pragma once

#include "InputModifiers.h"

#include "InputModifier_SettingDeadZone.generated.h"


//
// デッドゾーンがどのスティックに対応しているかのタイプ
//
UENUM()
enum class EDeadzoneStick : uint8
{
	// 移動用のスティック
	MoveStick = 0,

	// 視点用のスティック
	LookStick = 1,
};


 //
 // 設定に基づいてデッドゾーンを入力値に反映させる InputModifier クラス
 //
UCLASS(NotBlueprintable, meta = (DisplayName = "IM Settings Driven Dead Zone"))
class UInputModifier_SettingDeadZone : public UInputModifier
{
	GENERATED_BODY()
public:
	UInputModifier_SettingDeadZone() {}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DeadZone")
	EDeadzoneStick DeadzoneStick{ EDeadzoneStick::MoveStick };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DeadZone")
	EDeadZoneType Type{ EDeadZoneType::Radial };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DeadZone")
	float UpperThreshold{ 1.0f };

protected:
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;

	virtual FLinearColor GetVisualizationColor_Implementation(FInputActionValue SampleValue, FInputActionValue FinalValue) const override;

};
