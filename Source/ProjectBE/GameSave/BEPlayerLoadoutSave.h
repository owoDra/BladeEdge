// Copyright (C) 2024 owoDra

#pragma once

#include "PlayerSave.h"

#include "GameplayTagContainer.h"

#include "BEPlayerLoadoutSave.generated.h"


/**
 * プレイヤーのロードアウト設定を保存するためのゲームセーブクラス
 */
UCLASS(NotBlueprintable)
class PROJECTBE_API UBEPlayerLoadoutSave : public UPlayerSave
{
public:
	GENERATED_BODY()
public:
	UBEPlayerLoadoutSave() {}

	virtual FString GetSaveSlotName_Implementation() const override { return TEXT("Loadout"); }

	/////////////////////////////////////////////////////////////////////////////////////
	// セーブデータ
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skin", meta = (AllowedTypes = "Fighter"))
	TMap<FPrimaryAssetId, FName> FighterSkin;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skin", meta = (AllowedTypes = "Weapon"))
	TMap<FPrimaryAssetId, FName> WeaponSkin;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loadout", meta = (Categories = "Equipment.Slot"))
	TMap<FGameplayTag, FPrimaryAssetId> EquipmentItems;


	/////////////////////////////////////////////////////////////////////////////////////
	// 初期化など
public:
	virtual void ResetToDefault() override;

};

