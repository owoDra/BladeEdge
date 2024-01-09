// Copyright (C) 2024 owoDra

#include "InputModifier_GamepadSensitivity.h"

#include "GameSetting/ControlSettingSubsystem.h"

#include "GSCGameUserSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InputModifier_GamepadSensitivity)


FInputActionValue UInputModifier_GamepadSensitivity::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	if (CurrentValue.GetValueType() == EInputActionValueType::Boolean || !SensitivityLevelTable)
	{
		return CurrentValue;
	}
	
	auto* Settings{ UGSCGameUserSettings::GetSettingSubsystem<UControlSettingSubsystem>() };
	ensure(Settings);

	const auto Sensitivity{ (TargetingType == EGamepadTargetingType::Normal) ? Settings->GetGamepadLookSensitivityPreset() : Settings->GetGamepadTargetingSensitivityPreset() };

	const auto Scalar{ SensitivityLevelTable->SensitivtyEnumToFloat(Sensitivity) };

	return CurrentValue.Get<FVector>() * Scalar;
}
