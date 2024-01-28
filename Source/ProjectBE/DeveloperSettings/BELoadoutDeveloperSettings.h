// Copyright (C) 2024 owoDra

#pragma once

#include "Engine/DeveloperSettings.h"

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
	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere, Category = "Loadout", meta = (AllowedTypes = "Fighter"))
	FPrimaryAssetId DefaultFighter;

	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere, Category = "Loadout", meta = (AllowedTypes = "Weapon"))
	FPrimaryAssetId DefaultWeapon;

	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere, Category = "Loadout", meta = (AllowedTypes = "MainSkill"))
	FPrimaryAssetId DefaultMainSkill;

	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere, Category = "Loadout", meta = (AllowedTypes = "SubSKill"))
	FPrimaryAssetId DefaultSubSkill;

	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere, Category = "Loadout", meta = (AllowedTypes = "UltimateSkill"))
	FPrimaryAssetId DefaultUltimateSkill;

};

