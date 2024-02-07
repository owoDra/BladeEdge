// Copyright (C) 2024 owoDra

#pragma once

#include "Resolver/SettingUITypeResolver_Switch.h"

#include "AudioMixerBlueprintLibrary.h"

#include "BESettingUITypeResolver_AudioDevice.generated.h"


/**
 * Resolver class for "Switch(String)" type setting option
 */
UCLASS(Blueprintable, meta = (DisplayName = "Audio Device Switch Resolver"))
class UBESettingUITypeResolver_Switch_AudioDevice final : public USettingUITypeResolver_Switch
{
	GENERATED_BODY()
public:
	UBESettingUITypeResolver_Switch_AudioDevice(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	////////////////////////////////////////////////////////////////////////////
	// Template Functions
#if WITH_EDITOR
public:
	UFUNCTION()
	static FString GetterTemplate() { return FString(); }

	UFUNCTION()
	static void SetterTemplate(FString Arg) {}
#endif


	////////////////////////////////////////////////////////////////////////////
	// Initialization
public:
	virtual void OnInitialized() override;

	virtual void ReleaseResolver() override;


	////////////////////////////////////////////////////////////////////////////
	// Audio
protected:
	UPROPERTY(Transient)
	FOnAudioOutputDevicesObtained DevicesObtainedCallback;

	UPROPERTY(Transient)
	bool bAudioDeveiceReady{ false };

protected:
	void CheckAudioReady() const;

protected:
	UFUNCTION()
	void OnAudioOutputDevicesObtained(const TArray<FAudioOutputDeviceInfo>& AvailableDevices);

	UFUNCTION()
	void DeviceAddedOrRemoved(FString DeviceId);

	UFUNCTION()
	void DefaultDeviceChanged(EAudioDeviceChangedRole InRole, FString DeviceId);


	////////////////////////////////////////////////////////////////////////////
	// Getter・Setter
private:
	UPROPERTY(Transient)
	TArray<FText> OptionDisplayTexts;

	UPROPERTY(Transient)
	TArray<FString> OptionValues;

public:
	virtual void SetSettingValue(int32 NewValue) override;
	virtual int32 GetSettingValue() const override;

	virtual TArray<FText> GetOptionDisplayTexts() const override;

};
