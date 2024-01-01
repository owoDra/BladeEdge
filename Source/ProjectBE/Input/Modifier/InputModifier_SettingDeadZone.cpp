// Copyright (C) 2023 owoDra

#include "InputModifier_SettingDeadZone.h"

#include "GameSetting/ControlSettingSubsystem.h"

#include "GSCGameUserSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InputModifier_SettingDeadZone)


FInputActionValue UInputModifier_SettingDeadZone::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
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

	auto LowerThreshold
	{
		(DeadzoneStick == EDeadzoneStick::MoveStick) ? Settings->GetGamepadMoveStickDeadZone() : Settings->GetGamepadLookStickDeadZone()
	};
		
	LowerThreshold = FMath::Clamp(LowerThreshold, 0.0f, 1.0f);

	// デッドゾーンとする値を削除し、入力を +/- 1 の範囲に変換してスケーリングするラムダ関数
	
	auto DeadZoneLambda = [LowerThreshold, this](const float AxisVal)
	{
		return FMath::Min(1.f, (FMath::Max(0.f, FMath::Abs(AxisVal) - LowerThreshold) / (UpperThreshold - LowerThreshold))) * FMath::Sign(AxisVal);
	};

	// デッドゾーンを適用する

	auto NewValue{ CurrentValue.Get<FVector>() };

	switch (Type)
	{
	case EDeadZoneType::Axial:
		NewValue.X = DeadZoneLambda(NewValue.X);
		NewValue.Y = DeadZoneLambda(NewValue.Y);
		NewValue.Z = DeadZoneLambda(NewValue.Z);
		break;

	case EDeadZoneType::Radial:
		if (ValueType == EInputActionValueType::Axis3D)
		{
			NewValue = NewValue.GetSafeNormal() * DeadZoneLambda(NewValue.Size());
		}
		else if (ValueType == EInputActionValueType::Axis2D)
		{
			NewValue = NewValue.GetSafeNormal2D() * DeadZoneLambda(NewValue.Size2D());
		}
		else
		{
			NewValue.X = DeadZoneLambda(NewValue.X);
		}
		break;
	}

	return NewValue;
}

FLinearColor UInputModifier_SettingDeadZone::GetVisualizationColor_Implementation(FInputActionValue SampleValue, FInputActionValue FinalValue) const
{
	if (FinalValue.GetValueType() == EInputActionValueType::Boolean || FinalValue.GetValueType() == EInputActionValueType::Axis1D)
	{
		return FLinearColor(FinalValue.Get<float>() == 0.f ? 1.f : 0.f, 0.f, 0.f);
	}

	return FLinearColor((FinalValue.Get<FVector2D>().X == 0.f ? 0.5f : 0.f) + (FinalValue.Get<FVector2D>().Y == 0.f ? 0.5f : 0.f), 0.f, 0.f);
}
