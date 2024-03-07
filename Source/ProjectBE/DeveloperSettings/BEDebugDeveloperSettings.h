// Copyright (C) 2024 owoDra

#pragma once

#include "Engine/DeveloperSettings.h"

#include "GameplayTagContainer.h"

#include "BEDebugDeveloperSettings.generated.h"


/**
 * このゲームのゲームプレイデバッグ用のデベロッパー設定
 */
UCLASS(Config = "Game", Defaultconfig, meta = (DisplayName = "Debug"))
class UBEDebugDeveloperSettings : public UDeveloperSettings
{
public:
	GENERATED_BODY()
public:
	UBEDebugDeveloperSettings();


	///////////////////////////////////////////////
	// Game Phase
protected:
	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere, Category = "Game Phase")
	bool bUseFakeLobby{ false };

public:
	UFUNCTION(BlueprintCallable, Category = "Game Phase", meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool IsUsingFakeLobby() { return GetDefault<UBEDebugDeveloperSettings>()->bUseFakeLobby; }

};

