// Copyright (C) 2024 owoDra

#pragma once

#include "Item/BEEquipmentItemData.h"

#include "BEItemData_Weapon.generated.h"


/**
 * キャラクターが使用可能な武器を表すデータ
 */
UCLASS(meta = (DisplayName = "Weapon Data"))
class PROJECTBE_API UBEItemData_Weapon : public UBEEquipmentItemData
{
	GENERATED_BODY()
public:
	UBEItemData_Weapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//
	// このデータを表す PrimaryAssetType 名
	//
	inline static const FName NAME_WeaponDataType{ TEXTVIEW("Weapon") };

	virtual FGameplayTag GetSlotTag() const override;
};
