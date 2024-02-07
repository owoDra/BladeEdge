// Copyright (C) 2024 owoDra

#include "BESettingUITypeResolver_AudioDevice.h"

#include "AudioDeviceNotificationSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BESettingUITypeResolver_AudioDevice)


UBESettingUITypeResolver_Switch_AudioDevice::UBESettingUITypeResolver_Switch_AudioDevice(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


// Initialization

void UBESettingUITypeResolver_Switch_AudioDevice::OnInitialized()
{
	DevicesObtainedCallback.BindUFunction(this, FName(TEXT("OnAudioOutputDevicesObtained")));

	auto* AudioDeviceNotifSubsystem{ UAudioDeviceNotificationSubsystem::Get() };
	if (ensure(AudioDeviceNotifSubsystem))
	{
		AudioDeviceNotifSubsystem->DeviceAddedNative.AddUObject(this, &ThisClass::DeviceAddedOrRemoved);
		AudioDeviceNotifSubsystem->DeviceRemovedNative.AddUObject(this, &ThisClass::DeviceAddedOrRemoved);
		AudioDeviceNotifSubsystem->DefaultRenderDeviceChangedNative.AddUObject(this, &ThisClass::DefaultDeviceChanged);
	}

	UAudioMixerBlueprintLibrary::GetAvailableAudioOutputDevices(this, DevicesObtainedCallback);
}

void UBESettingUITypeResolver_Switch_AudioDevice::ReleaseResolver()
{
	DevicesObtainedCallback.Clear();

	if (auto* AudioDeviceNotifSubsystem{ UAudioDeviceNotificationSubsystem::Get() })
	{
		AudioDeviceNotifSubsystem->DeviceAddedNative.RemoveAll(this);
		AudioDeviceNotifSubsystem->DeviceRemovedNative.RemoveAll(this);
		AudioDeviceNotifSubsystem->DefaultRenderDeviceChangedNative.RemoveAll(this);
	}

	Super::ReleaseResolver();
}


// Audio

void UBESettingUITypeResolver_Switch_AudioDevice::CheckAudioReady() const
{
	if (!bAudioDeveiceReady)
	{
		UAudioMixerBlueprintLibrary::GetAvailableAudioOutputDevices(this, DevicesObtainedCallback);
	}
}

void UBESettingUITypeResolver_Switch_AudioDevice::OnAudioOutputDevicesObtained(const TArray<FAudioOutputDeviceInfo>& AvailableDevices)
{
	bAudioDeveiceReady = !AvailableDevices.IsEmpty();

	OptionDisplayTexts.Reset();
	OptionValues.Reset();

	OptionDisplayTexts.Add(FText::GetEmpty());
	OptionValues.Add(TEXT(""));

	FString SystemDefaultDeviceId;
	FString SystemDefaultDeviceName;
	FString SystemCurrentDeviceId;

	for (const auto& DeviceInfo : AvailableDevices)
	{
		if (!DeviceInfo.DeviceId.IsEmpty() && !DeviceInfo.Name.IsEmpty())
		{
			// System Default 

			if (DeviceInfo.bIsSystemDefault)
			{
				SystemDefaultDeviceId = DeviceInfo.DeviceId;
				SystemDefaultDeviceName = DeviceInfo.Name;
			}

			// Current Device

			if (DeviceInfo.bIsCurrentDevice)
			{
				SystemCurrentDeviceId = DeviceInfo.DeviceId;
			}

			// Add the menu option

			OptionDisplayTexts.Add(FText::FromString(DeviceInfo.Name));
			OptionValues.Add(DeviceInfo.DeviceId);
		}
	}

	OptionDisplayTexts[0] = FText::Format(NSLOCTEXT("SettingUI", "DefaultAudioOutputDevice", "Default Output - {0}"), FText::FromString(SystemDefaultDeviceName));
	
	UpdateEditableState();
	NotifyPropertyOptionChange();
	NotifyPropertyValueChange();
}

void UBESettingUITypeResolver_Switch_AudioDevice::DeviceAddedOrRemoved(FString DeviceId)
{
	UAudioMixerBlueprintLibrary::GetAvailableAudioOutputDevices(this, DevicesObtainedCallback);
}

void UBESettingUITypeResolver_Switch_AudioDevice::DefaultDeviceChanged(EAudioDeviceChangedRole InRole, FString DeviceId)
{
	UAudioMixerBlueprintLibrary::GetAvailableAudioOutputDevices(this, DevicesObtainedCallback);
}


// Setting

void UBESettingUITypeResolver_Switch_AudioDevice::SetSettingValue(int32 NewValue)
{
	CheckAudioReady();

	if (ensure(OptionValues.IsValidIndex(NewValue)))
	{
		SetPropertyValue<FString>(OptionValues[NewValue]);
	}
}

int32 UBESettingUITypeResolver_Switch_AudioDevice::GetSettingValue() const
{
	CheckAudioReady();

	FString Result;
	GetPropertyValue<FString>(Result);

	const auto FoundIndex{ OptionValues.Find(Result) };

	return (FoundIndex == INDEX_NONE) ? 0 : FoundIndex;
}


TArray<FText> UBESettingUITypeResolver_Switch_AudioDevice::GetOptionDisplayTexts() const
{
	return OptionDisplayTexts;
}
