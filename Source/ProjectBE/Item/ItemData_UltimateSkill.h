// Copyright (C) 2024 owoDra

#pragma once

#include "ItemData.h"

#include "ItemData_UltimateSkill.generated.h"


/**
 * キャラクターが使用可能なアルティメットスキルを表すデータ
 */
UCLASS(meta = (DisplayName = "Ultimate Skill Data"))
class PROJECTBE_API UItemData_UltimateSkill : public UItemData
{
	GENERATED_BODY()
public:
	UItemData_UltimateSkill(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//
	// このデータを表す PrimaryAssetType 名
	//
	inline static const FName NAME_UltimateSkillDataType{ TEXTVIEW("UltimateSkill") };

};
