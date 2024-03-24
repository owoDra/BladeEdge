// Copyright (C) 2024 owoDra

#include "GameplaySettingSubsystem.h"

#include "Framework/Application/SlateApplication.h"
#include "Internationalization/Culture.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameplaySettingSubsystem)


UGameplaySettingSubsystem::UGameplaySettingSubsystem()
{
	FInternationalization::Get().OnCultureChanged().AddUObject(this, &ThisClass::OnCultureChanged);
}


void UGameplaySettingSubsystem::SetToDefaults()
{
    ColorBlindMode = EColorBlindMode::Off;
    ColorBlindStrength = 10;

	ClearPendingCulture();
	bResetToDefaultCulture = true;
}

void UGameplaySettingSubsystem::ApplySettings()
{
    ApplyCultureSettings();
    ApplyColorBlindSettings();
}


void UGameplaySettingSubsystem::OnCultureChanged()
{
    ClearPendingCulture();
    bResetToDefaultCulture = false;
}

void UGameplaySettingSubsystem::ClearPendingCulture()
{
    PendingCulture.Reset();
}

bool UGameplaySettingSubsystem::IsUsingDefaultCulture() const
{
    FString Culture;
    GConfig->GetString(TEXT("Internationalization"), TEXT("Culture"), Culture, GGameUserSettingsIni);

    return Culture.IsEmpty();
}

void UGameplaySettingSubsystem::ResetToDefaultCulture()
{
    ClearPendingCulture();
    bResetToDefaultCulture = true;
    MarkDirty();
}

void UGameplaySettingSubsystem::ApplyCultureSettings()
{
	if (bResetToDefaultCulture)
	{
		// 言語をデフォルトに設定

		const auto SystemDefaultCulture{ FInternationalization::Get().GetDefaultCulture().ToSharedPtr() };
		check(SystemDefaultCulture.IsValid());

		const auto CultureToApply{ SystemDefaultCulture->GetName() };
		if (FInternationalization::Get().SetCurrentCulture(CultureToApply))
		{
			// Config から設定を削除

			GConfig->RemoveKey(TEXT("Internationalization"), TEXT("Culture"), GGameUserSettingsIni);
			GConfig->Flush(false, GGameUserSettingsIni);
		}

		bResetToDefaultCulture = false;
	}
	else if (!PendingCulture.IsEmpty())
	{
		// 適用中に PendingCulture がクリアされるためここでコピーを作成

		const auto CultureToApply{ PendingCulture };

		// PendingCulture を適用

		if (FInternationalization::Get().SetCurrentCulture(CultureToApply))
		{
			// Config に書き込み、エンジン初期化時から読み込まれるようにする

			GConfig->SetString(TEXT("Internationalization"), TEXT("Culture"), *CultureToApply, GGameUserSettingsIni);
			GConfig->Flush(false, GGameUserSettingsIni);
		}

		ClearPendingCulture();
	}
}


void UGameplaySettingSubsystem::ApplyColorBlindSettings()
{
    FSlateApplication::Get().GetRenderer()->SetColorVisionDeficiencyType(
        (EColorVisionDeficiency)(int32)ColorBlindMode, (int32)ColorBlindStrength, true, false);
}
