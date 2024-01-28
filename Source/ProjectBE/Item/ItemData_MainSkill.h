// Copyright (C) 2024 owoDra

#pragma once

#include "ItemData.h"

#include "ItemData_MainSkill.generated.h"


/**
 * キャラクターが使用可能なメインスキルを表すデータ
 */
UCLASS(meta = (DisplayName = "Main Skill Data"))
class PROJECTBE_API UItemData_MainSkill : public UItemData
{
	GENERATED_BODY()
public:
	UItemData_MainSkill(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//
	// このデータを表す PrimaryAssetType 名
	//
	inline static const FName NAME_MainSkillDataType{ TEXTVIEW("MainSkill") };

};
