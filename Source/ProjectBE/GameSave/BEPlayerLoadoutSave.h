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
	FPrimaryAssetId Fighter;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skin", meta = (AllowedTypes = "Weapon"))
	FPrimaryAssetId Weapon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skin", meta = (AllowedTypes = "MainSkill"))
	FPrimaryAssetId MainSkill;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skin", meta = (AllowedTypes = "SubSkill"))
	FPrimaryAssetId SubSkill;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skin", meta = (AllowedTypes = "UltimateSkill"))
	FPrimaryAssetId UltimateSkill;

	/////////////////////////////////////////////////////////////////////////////////////
	// 初期化など
public:
	virtual void ResetToDefault() override;

};

