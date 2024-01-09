// Copyright (C) 2024 owoDra

#include "InputModifier_AimInversion.h"

#include "GameSetting/ControlSettingSubsystem.h"

#include "GSCGameUserSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InputModifier_AimInversion)


FInputActionValue UInputModifier_AimInversion::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	auto* Settings{ UGSCGameUserSettings::GetSettingSubsystem<UControlSettingSubsystem>() };	
	ensure(Settings);

	FVector NewValue = CurrentValue.Get<FVector>();
	
	if (Settings->GetInvertVerticalAxis())
	{
		NewValue.Y *= -1.0f;
	}
	
	if (Settings->GetInvertHorizontalAxis())
	{
		NewValue.X *= -1.0f;
	}
	
	return NewValue;
}
