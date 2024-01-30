// Copyright (C) 2024 owoDra

#pragma once

#include "Engine/DeveloperSettings.h"

#include "GameplayTagContainer.h"

#include "BELoadoutDeveloperSettings.generated.h"


/**
 * このゲームのロードアウト関係のデベロッパー設定
 */
UCLASS(Config = "Game", Defaultconfig, meta = (DisplayName = "Loadout"))
class UBELoadoutDeveloperSettings : public UDeveloperSettings
{
public:
	GENERATED_BODY()
public:
	UBELoadoutDeveloperSettings();

	///////////////////////////////////////////////
	// デフォルトデータ
public:
	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere, Category = "Default", meta = (AllowedTypes = "Fighter"))
	FPrimaryAssetId Fighter;

	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere, Category = "Default", meta = (AllowedTypes = "Weapon"))
	FPrimaryAssetId Weapon;

	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere, Category = "Default", meta = (AllowedTypes = "MainSkill"))
	FPrimaryAssetId MainSkill;

	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere, Category = "Default", meta = (AllowedTypes = "SubSkill"))
	FPrimaryAssetId SubSkill;

	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere, Category = "Default", meta = (AllowedTypes = "UltimateSkill"))
	FPrimaryAssetId UltimateSkill;

};

