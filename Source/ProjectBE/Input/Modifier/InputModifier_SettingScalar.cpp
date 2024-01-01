// Copyright (C) 2023 owoDra

#include "InputModifier_SettingScalar.h"

#include "GameSetting/ControlSettingSubsystem.h"

#include "GSCGameUserSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InputModifier_SettingScalar)


FInputActionValue UInputModifier_SettingScalar::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
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

	auto SettingsClass{ Settings->GetClass() };

	const auto bHasCachedProperty{ PropertyCache.Num() == 3 };

	const auto* XAxisValue{ bHasCachedProperty ? PropertyCache[0] : SettingsClass->FindPropertyByName(XAxisScalarSettingName) };
	const auto* YAxisValue{ bHasCachedProperty ? PropertyCache[1] : SettingsClass->FindPropertyByName(YAxisScalarSettingName) };
	const auto* ZAxisValue{ bHasCachedProperty ? PropertyCache[2] : SettingsClass->FindPropertyByName(ZAxisScalarSettingName) };

	if (PropertyCache.IsEmpty())
	{
		PropertyCache.Emplace(XAxisValue);
		PropertyCache.Emplace(YAxisValue);
		PropertyCache.Emplace(ZAxisValue);
	}

	auto ScalarToUse{ FVector(1.0, 1.0, 1.0) };

	switch (CurrentValue.GetValueType())
	{
	case EInputActionValueType::Axis3D:
		ScalarToUse.Z = ZAxisValue ? *ZAxisValue->ContainerPtrToValuePtr<double>(Settings) : 1.0;
		//[[fallthrough]];

	case EInputActionValueType::Axis2D:
		ScalarToUse.Y = YAxisValue ? *YAxisValue->ContainerPtrToValuePtr<double>(Settings) : 1.0;
		//[[fallthrough]];

	case EInputActionValueType::Axis1D:
		ScalarToUse.X = XAxisValue ? *XAxisValue->ContainerPtrToValuePtr<double>(Settings) : 1.0;
		break;
	}

	ScalarToUse.X = FMath::Clamp(ScalarToUse.X, MinValueClamp.X, MaxValueClamp.X);
	ScalarToUse.Y = FMath::Clamp(ScalarToUse.Y, MinValueClamp.Y, MaxValueClamp.Y);
	ScalarToUse.Z = FMath::Clamp(ScalarToUse.Z, MinValueClamp.Z, MaxValueClamp.Z);

	// 設定値を反映させる

	return CurrentValue.Get<FVector>() * ScalarToUse;
}
