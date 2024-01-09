// Copyright (C) 2024 owoDra

#include "AudioSettingSubsystem.h"

#include "Audio/BEAudioMixSubsystem.h"
#include "GameplayTag/BETags_Platform.h"

#include "GameplayTag/GACTags_Audio.h"

#include "ICommonUIModule.h"
#include "CommonUISettings.h"
#include "Framework/Application/SlateApplication.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AudioSettingSubsystem)


void UAudioSettingSubsystem::SetToDefaults()
{
    AudioOutputDeviceId = FString();

    bUseHeadphoneMode = false;

    bUseHDRAudioMode = false;

    AllowAudioInBackground = EAllowBackgroundAudioType::Off;

    OverallVolume = 1.0f;
    MusicVolume = 1.0f;
    SoundFXVolume = 1.0f;
}

void UAudioSettingSubsystem::ApplySettings()
{
    ApplyHeadphoneModeSetting();
	ApplyHDRAudioModeSetting();
	ApplyAllowAudioInBackgroundSetting();
	ApplySoundVolumeSettings();
}


void UAudioSettingSubsystem::SetAudioOutputDeviceId(const FString& InAudioOutputDeviceId)
{
    ChangeValueAndDirty(AudioOutputDeviceId, InAudioOutputDeviceId);

    OnAudioOutputDeviceChanged.Broadcast(InAudioOutputDeviceId);
}


void UAudioSettingSubsystem::SetHeadphoneModeEnabled(bool bEnabled)
{
	if (CanModifyHeadphoneModeEnabled())
	{
		ChangeValueAndDirty(bUseHeadphoneMode, bEnabled);
	}
}

bool UAudioSettingSubsystem::CanModifyHeadphoneModeEnabled() const
{
	static auto* BinauralSpatializationDisabledCVar{ IConsoleManager::Get().FindConsoleVariable(TEXT("au.DisableBinauralSpatialization")) };
	const auto bHRTFOptionAvailable{ BinauralSpatializationDisabledCVar && ((BinauralSpatializationDisabledCVar->GetFlags() & EConsoleVariableFlags::ECVF_SetByMask) <= EConsoleVariableFlags::ECVF_SetByGameSetting) };

	const auto bBinauralSettingControlledByOS{ ICommonUIModule::GetSettings().GetPlatformTraits().HasTag(TAG_Platform_Trait_BinauralSettingControlledByOS) };

	return bHRTFOptionAvailable && !bBinauralSettingControlledByOS;
}

void UAudioSettingSubsystem::ApplyHeadphoneModeSetting()
{
    static auto* BinauralSpatializationDisabledCVar{ IConsoleManager::Get().FindConsoleVariable(TEXT("au.DisableBinauralSpatialization")) };
	
    if (BinauralSpatializationDisabledCVar)
	{
		BinauralSpatializationDisabledCVar->Set(!bUseHeadphoneMode, ECVF_SetByGameSetting);
	}
}


void UAudioSettingSubsystem::SetHDRAudioModeEnabled(bool bEnabled)
{
	ChangeValueAndDirty(bUseHDRAudioMode, bEnabled);
}

void UAudioSettingSubsystem::ApplyHDRAudioModeSetting()
{
	if (GEngine)
	{
		if (const auto* World{ GEngine->GetCurrentPlayWorld() })
		{
			if (auto* AudioMixSubsystem{ World->GetSubsystem<UBEAudioMixSubsystem>() })
			{
				AudioMixSubsystem->ApplyDynamicRangeEffectsChains(bUseHDRAudioMode);
			}
		}
	}
}


void UAudioSettingSubsystem::SetAllowAudioInBackgroundSetting(EAllowBackgroundAudioType NewValue)
{
    ChangeValueAndDirty(AllowAudioInBackground, NewValue);
}

void UAudioSettingSubsystem::ApplyAllowAudioInBackgroundSetting()
{
	FApp::SetUnfocusedVolumeMultiplier((AllowAudioInBackground != EAllowBackgroundAudioType::Off) ? 1.0f : 0.0f);
}


void UAudioSettingSubsystem::SetOverallVolume(float InVolume)
{
    ChangeValueAndDirty(OverallVolume, InVolume);

	ApplySoundVolumeSetting(TAG_Audio_ControlBus_Overrall, InVolume);
}

void UAudioSettingSubsystem::SetMusicVolume(float InVolume)
{
    ChangeValueAndDirty(MusicVolume, InVolume);

	ApplySoundVolumeSetting(TAG_Audio_ControlBus_Music, InVolume);
}

void UAudioSettingSubsystem::SetSoundFXVolume(float InVolume)
{
    ChangeValueAndDirty(SoundFXVolume, InVolume);

	ApplySoundVolumeSetting(TAG_Audio_ControlBus_SoundFX, InVolume);
}

void UAudioSettingSubsystem::ApplySoundVolumeSetting(FGameplayTag Tag, float InVolume)
{
	if (GEngine)
	{
		if (const auto* World{ GEngine->GetCurrentPlayWorld() })
		{
			if (auto* AudioMixSubsystem{ World->GetSubsystem<UBEAudioMixSubsystem>() })
			{
				AudioMixSubsystem->ApplyControlBusVolume(Tag, InVolume);
			}
		}
	}
}

void UAudioSettingSubsystem::ApplySoundVolumeSettings()
{
	if (GEngine)
	{
		if (const auto* World{ GEngine->GetCurrentPlayWorld() })
		{
			if (auto* AudioMixSubsystem{ World->GetSubsystem<UBEAudioMixSubsystem>() })
			{
				AudioMixSubsystem->ApplyControlBusVolumes(this);
			}
		}
	}
}
