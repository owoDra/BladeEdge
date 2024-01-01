// Copyright (C) 2023 owoDra

#include "BEAudioMixSubsystem.h"

// Project BE
#include "GameSetting/AudioSettingSubsystem.h"

// Game Setting Core
#include "GSCGameUserSettings.h"

// Game Audio Core
#include "GameplayTag/GACTags_Audio.h"

// Game UI Extension
#include "LoadingScreen/LoadingScreenManager.h"

// Engine Features
#include "AudioModulationStatics.h"
#include "AudioMixerBlueprintLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEAudioMixSubsystem)


void UBEAudioMixSubsystem::Deinitialize()
{
	if (auto* LoadingScreenManager{ UGameInstance::GetSubsystem<ULoadingScreenManager>(GetWorld()->GetGameInstance()) })
	{
		LoadingScreenManager->LoadingScreenVisibilityChanged.RemoveAll(this);
		ApplyOrRemoveLoadingScreenMix(false);
	}

	Super::Deinitialize();
}

void UBEAudioMixSubsystem::PostInitialize()
{
	Super::PostInitialize();

	if (auto* LoadingScreenManager{ UGameInstance::GetSubsystem<ULoadingScreenManager>(GetWorld()->GetGameInstance()) })
	{
		LoadingScreenManager->LoadingScreenVisibilityChanged.AddUObject(this, &ThisClass::OnLoadingScreenStatusChanged);
		ApplyOrRemoveLoadingScreenMix(LoadingScreenManager->IsCurrentlyShowingLoadingScreen());
	}
}

void UBEAudioMixSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	if (const auto* World{ InWorld.GetWorld() })
	{
		ApplyControlBusMix(TAG_Audio_ControlBusMix_DefaultBase);
		ApplyControlBusMix(TAG_Audio_ControlBusMix_User);

		if (const auto* GameSetting{ UGSCGameUserSettings::GetSettingSubsystem<UAudioSettingSubsystem>() })
		{
			ApplyControlBusVolumes(GameSetting);

			ApplyDynamicRangeEffectsChains(GameSetting->IsHDRAudioModeEnabled());
		}
	}
}


void UBEAudioMixSubsystem::ApplyControlBusVolumes(const UAudioSettingSubsystem* Setting)
{
	check(Setting);

	if (auto* World{ GetWorld() })
	{
		if (auto ControlBusMix{ ControlBusMixMap.FindRef(TAG_Audio_ControlBusMix_User) })
		{
			TArray<FSoundControlBusMixStage> ControlBusMixStageArray;

			// Overrall

			if (auto ControlBus{ ControlBusMap.FindRef(TAG_Audio_ControlBus_Overrall) })
			{
				const auto NewControlBusMixStage
				{
					UAudioModulationStatics::CreateBusMixStage(World, ControlBus, Setting->GetOverallVolume())
				};

				ControlBusMixStageArray.Add(NewControlBusMixStage);
			}

			// Music

			if (auto ControlBus{ ControlBusMap.FindRef(TAG_Audio_ControlBus_Music) })
			{
				const auto NewControlBusMixStage
				{
					UAudioModulationStatics::CreateBusMixStage(World, ControlBus, Setting->GetMusicVolume())
				};

				ControlBusMixStageArray.Add(NewControlBusMixStage);
			}

			// SoundFX

			if (auto ControlBus{ ControlBusMap.FindRef(TAG_Audio_ControlBus_SoundFX) })
			{
				const auto NewControlBusMixStage
				{
					UAudioModulationStatics::CreateBusMixStage(World, ControlBus, Setting->GetSoundFXVolume())
				};

				ControlBusMixStageArray.Add(NewControlBusMixStage);
			}

			UAudioModulationStatics::UpdateMix(World, ControlBusMix, ControlBusMixStageArray);
		}
	}
}


void UBEAudioMixSubsystem::OnLoadingScreenStatusChanged(bool bShowingLoadingScreen)
{
	ApplyOrRemoveLoadingScreenMix(bShowingLoadingScreen);
}

void UBEAudioMixSubsystem::ApplyOrRemoveLoadingScreenMix(bool bWantsLoadingScreenMix)
{
	if (auto* World{ GetWorld() })
	{
		if (auto ControlBusMix{ ControlBusMixMap.FindRef(TAG_Audio_ControlBusMix_Loading) })
		{
			if (bWantsLoadingScreenMix)
			{
				// Apply the mix
				UAudioModulationStatics::ActivateBusMix(World, ControlBusMix);
			}
			else
			{
				// Remove the mix
				UAudioModulationStatics::DeactivateBusMix(World, ControlBusMix);
			}
		}
	}
}
