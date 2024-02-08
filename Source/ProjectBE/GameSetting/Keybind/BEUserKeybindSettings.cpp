// Copyright (C) 2024 owoDra

#include "BEUserKeybindSettings.h"

#include "GameplayTag/BETags_UI.h"

// Game UI: Setting Addon
#include "Register/SettingUICustomRegister.h"
#include "SettingUIDeveloperSettings.h"
#include "SettingUISubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEUserKeybindSettings)


void UBEUserKeybindSettings::OnKeyMappingRegistered(FPlayerKeyMapping& RegisteredMapping, const FEnhancedActionKeyMapping& SourceMapping)
{
	if (auto* Subsystem{ ULocalPlayer::GetSubsystem<USettingUISubsystem>(GetLocalPlayer()) })
	{
		Subsystem->RemoveSettingTable(TAG_UI_SettingTable_KeyboardAndMouse);
		Subsystem->RemoveSettingTable(TAG_UI_SettingTable_GamePad);

		const auto* DevSetting{ GetDefault<USettingUIDeveloperSettings>() };

		if (auto* Table_KM{ DevSetting->StartupSettingTables.Find(TAG_UI_SettingTable_KeyboardAndMouse) })
		{
			if (const auto* CustomRegister{ Cast<USettingUICustomRegister>(Table_KM->TryLoad()) })
			{
				Subsystem->AddSettingCustomTable(TAG_UI_SettingTable_KeyboardAndMouse, CustomRegister);
			}
		}

		if (auto* Table_GP{ DevSetting->StartupSettingTables.Find(TAG_UI_SettingTable_GamePad) })
		{
			if (const auto* CustomRegister{ Cast<USettingUICustomRegister>(Table_GP->TryLoad()) })
			{
				Subsystem->AddSettingCustomTable(TAG_UI_SettingTable_GamePad, CustomRegister);
			}
		}
	}
}
