// Copyright (C) 2023 owoDra

#include "ControlSettingSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ControlSettingSubsystem)


void UControlSettingSubsystem::SetToDefaults()
{
    bInvertVerticalAxis = false;
    bInvertHorizontalAxis = false;

    MouseSensitivityX = 1.0f;
    MouseSensitivityY = 1.0f;
    TargetingMultiplier = 1.0f;

    GamepadLookSensitivityPreset = EGamepadSensitivity::Normal;
    GamepadTargetingSensitivityPreset = EGamepadSensitivity::Normal;

    GamepadMoveStickDeadZone = 0.0f;
    GamepadLookStickDeadZone = 0.0f;

    bForceFeedbackEnabled = true;

    bTriggerHapticsEnabled = false;
    bTriggerPullUsesHapticThreshold = true;
    TriggerHapticStrength = 8;
    TriggerHapticStartPosition = 0;
}
