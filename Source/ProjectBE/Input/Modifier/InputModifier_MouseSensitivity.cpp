// Copyright (C) 2024 owoDra

#include "InputModifier_MouseSensitivity.h"

#include "GameSetting/ControlSettingSubsystem.h"

#include "GSCGameUserSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InputModifier_MouseSensitivity)


FInputActionValue UInputModifier_MouseSensitivity::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	auto ValueType{ CurrentValue.GetValueType() };

	// ValueType が Boolean だったらスキップ

	if (ValueType == EInputActionValueType::Boolean)
	{
		return CurrentValue;
	}

	// 設定を取得

	auto* Settings{ UGSCGameUserSettings::GetSettingSubsystem<UControlSettingSubsystem>() };
	ensure(Settings);

	auto ScalarToUse
	{ 
		FVector(
			Settings->GetMouseSensitivityX(),
			Settings->GetMouseSensitivityY(),
			1.0)
	};

	// 設定値を反映させる

	return CurrentValue.Get<FVector>() * ScalarToUse;
}
