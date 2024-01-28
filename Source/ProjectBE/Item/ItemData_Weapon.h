// Copyright (C) 2024 owoDra

#pragma once

#include "ItemData.h"

#include "ItemData_Weapon.generated.h"


/**
 * キャラクターが使用可能な武器を表すデータ
 */
UCLASS(meta = (DisplayName = "Weapon Data"))
class PROJECTBE_API UItemData_Weapon : public UItemData
{
	GENERATED_BODY()
public:
	UItemData_Weapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//
	// このデータを表す PrimaryAssetType 名
	//
	inline static const FName NAME_WeaponDataType{ TEXTVIEW("Weapon") };

};
