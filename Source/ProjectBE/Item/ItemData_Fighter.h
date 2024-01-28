// Copyright (C) 2024 owoDra

#pragma once

#include "ItemData.h"

#include "ItemData_Fighter.generated.h"


/**
 * 使用するファイター(アバター)を表すデータ
 */
UCLASS(meta = (DisplayName = "Fighter Data"))
class PROJECTBE_API UItemData_Fighter : public UItemData
{
	GENERATED_BODY()
public:
	UItemData_Fighter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//
	// このデータを表す PrimaryAssetType 名
	//
	inline static const FName NAME_FighterDataType{ TEXTVIEW("Fighter") };

};
