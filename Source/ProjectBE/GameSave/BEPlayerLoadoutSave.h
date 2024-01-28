// Copyright (C) 2024 owoDra

#pragma once

#include "PlayerSave.h"

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

	static const FName NAME_LoadoutLoading;


	/////////////////////////////////////////////////////////////////////////////////////
	// セーブデータ
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skin", meta = (AllowedTypes = "Fighter"))
	TMap<FPrimaryAssetId, FName> FighterSkin;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skin", meta = (AllowedTypes = "Weapon"))
	TMap<FPrimaryAssetId, FName> WeaponSkin;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loadout", meta = (AllowedTypes = "Fighter"))
	FPrimaryAssetId Fighter;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loadout", meta = (AllowedTypes = "Weapon"))
	FPrimaryAssetId Weapon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loadout", meta = (AllowedTypes = "MainSkill"))
	FPrimaryAssetId MainSkill;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loadout", meta = (AllowedTypes = "SubSKill"))
	FPrimaryAssetId SubSkill;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Loadout", meta = (AllowedTypes = "UltimateSkill"))
	FPrimaryAssetId UltimateSkill;


	/////////////////////////////////////////////////////////////////////////////////////
	// 初期化など
public:
	virtual void ResetToDefault() override;
	virtual void HandlePreSave() override;
	virtual void HandlePostSave(bool bSuccess) override;

};

