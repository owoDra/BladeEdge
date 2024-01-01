// Copyright (C) 2023 owoDra

#pragma once

#include "AudioMixSubsystem.h"

#include "BEAudioMixSubsystem.generated.h"

class UAudioSettingSubsystem;


/**
 * AudioMixSubsystem にゲーム設定との連携機能を追加したサブシステムクラス
 */
UCLASS()
class PROJECTBE_API UBEAudioMixSubsystem : public UAudioMixSubsystem
{
	GENERATED_BODY()
public:
	UBEAudioMixSubsystem() {}

public:
	virtual void Deinitialize() override;
	virtual void PostInitialize() override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

public:
	/**
	 * ゲーム設定を ControlBus の音量に適用する
	 */
	void ApplyControlBusVolumes(const UAudioSettingSubsystem* Setting);


protected:
	void OnLoadingScreenStatusChanged(bool bShowingLoadingScreen);
	void ApplyOrRemoveLoadingScreenMix(bool bWantsLoadingScreenMix);

};
