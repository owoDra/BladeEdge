// Copyright (C) 2023 owoDra

#pragma once

#include "GSCSubsystem.h"

#include "GameplayTagContainer.h"

#include "AudioSettingSubsystem.generated.h"

class USoundControlBus;
class USoundControlBusMix;


/**
 *  バックグラウンドサウンドタイプ
 */
UENUM(BlueprintType)
enum class EAllowBackgroundAudioType : uint8
{
	Off,
	AllSounds,

	Num UMETA(Hidden)
};


/**
 * オーディオ設定
 */
UCLASS(configdonotcheckdefaults)
class PROJECTBE_API UAudioSettingSubsystem : public UGSCSubsystem
{
	GENERATED_BODY()
public:
	UAudioSettingSubsystem() {}

protected:
	virtual FString GetCustomConfigIniName() const override { return FString(TEXT("UserGameplaySettings")); }

protected:
	virtual void SetToDefaults() override;
	virtual void ApplySettings() override;

	
public:
	/**
	 *  音響出力デバイスが変更されたとき
	 */
	DECLARE_EVENT_OneParam(UAudioSettingSubsystem, FAudioDeviceChangedDelegate, const FString& /*DeviceId*/);
	FAudioDeviceChangedDelegate OnAudioOutputDeviceChanged;


	// ===== オーディオデバイス　=====
private:
	// 音声出力デバイスのID
	UPROPERTY(Config) FString AudioOutputDeviceId;

public:
	UFUNCTION() FString GetAudioOutputDeviceId() const { return AudioOutputDeviceId; }
	UFUNCTION() void SetAudioOutputDeviceId(const FString& InAudioOutputDeviceId);


	// ===== 立体音響　=====
private:
	// 立体音響を利用するかどうか
	UPROPERTY(config) bool bUseHeadphoneMode;

public:
	UFUNCTION() bool IsHeadphoneModeEnabled() const { return bUseHeadphoneMode; }
	UFUNCTION() void SetHeadphoneModeEnabled(bool bEnabled);
	UFUNCTION() bool CanModifyHeadphoneModeEnabled() const;

protected:
	void ApplyHeadphoneModeSetting();


	// ===== HDR音響　=====
private:
	// HDR オーディオを使用するかどうか
	UPROPERTY(config) bool bUseHDRAudioMode;

public:
	UFUNCTION() bool IsHDRAudioModeEnabled() const { return bUseHDRAudioMode; }
	UFUNCTION() void SetHDRAudioModeEnabled(bool bEnabled);

protected:
	void ApplyHDRAudioModeSetting();


	// ===== バックグラウンド再生　=====
private:
	// バックグランドで音楽を再生し続けるかどうか
	UPROPERTY(config) EAllowBackgroundAudioType AllowAudioInBackground { EAllowBackgroundAudioType::Off };

public:
	UFUNCTION() EAllowBackgroundAudioType GetAllowAudioInBackgroundSetting() const { return AllowAudioInBackground; }
	UFUNCTION() void SetAllowAudioInBackgroundSetting(EAllowBackgroundAudioType NewValue);

protected:
	void ApplyAllowAudioInBackgroundSetting();


	// ===== 音量　=====
private:
	// それぞれのサウンドボリューム
	UPROPERTY(config) float OverallVolume{ 1.0f };
	UPROPERTY(config) float MusicVolume{ 1.0f };
	UPROPERTY(config) float SoundFXVolume{ 1.0f };

public:
	UFUNCTION() float GetOverallVolume() const { return OverallVolume; }
	UFUNCTION() void SetOverallVolume(float InVolume);

	UFUNCTION() float GetMusicVolume() const { return MusicVolume; }
	UFUNCTION() void SetMusicVolume(float InVolume);

	UFUNCTION() float GetSoundFXVolume() const { return SoundFXVolume; }
	UFUNCTION() void SetSoundFXVolume(float InVolume);

protected:
	void ApplySoundVolumeSetting(FGameplayTag Tag, float InVolume);
	void ApplySoundVolumeSettings();

};
