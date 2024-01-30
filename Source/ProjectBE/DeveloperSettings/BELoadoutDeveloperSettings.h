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
	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere, Category = "Loadout", meta = (ForceInlineRow, Categories = "Equipment.Slot"))
	TMap<FGameplayTag, FPrimaryAssetId> DefaultEquipmentItems;

public:
	/**
	 * DeveloperSettings で設定した各装備スロットに対応するデフォルト装備アイテムの PrimaryAssetId を返す。
	 */
	static FPrimaryAssetId GetEquipmentItemByTag(const FGameplayTag& SlotTag);

};

