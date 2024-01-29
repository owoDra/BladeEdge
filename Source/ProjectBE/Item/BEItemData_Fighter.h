// Copyright (C) 2024 owoDra

#pragma once

#include "Item/BEEquipmentItemData.h"

#include "BEItemData_Fighter.generated.h"


/**
 * 使用するファイター(アバター)を表すデータ
 */
UCLASS(meta = (DisplayName = "Fighter Data"))
class PROJECTBE_API UBEItemData_Fighter : public UBEEquipmentItemData
{
	GENERATED_BODY()
public:
	UBEItemData_Fighter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//
	// このデータを表す PrimaryAssetType 名
	//
	inline static const FName NAME_FighterDataType{ TEXTVIEW("Fighter") };

	virtual FGameplayTag GetSlotTag() const override;
};
