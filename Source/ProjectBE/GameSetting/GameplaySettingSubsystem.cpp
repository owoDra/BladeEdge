// Copyright (C) 2023 owoDra

#include "GameplaySettingSubsystem.h"

#include "Framework/Application/SlateApplication.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameplaySettingSubsystem)


void UGameplaySettingSubsystem::SetToDefaults()
{
    ColorBlindMode = EColorBlindMode::Off;
    ColorBlindStrength = 10;
}

void UGameplaySettingSubsystem::ApplySettings()
{
    ApplyColorBlindSettings();
}


void UGameplaySettingSubsystem::ApplyColorBlindSettings()
{
    FSlateApplication::Get().GetRenderer()->SetColorVisionDeficiencyType(
        (EColorVisionDeficiency)(int32)ColorBlindMode, (int32)ColorBlindStrength, true, false);
}
