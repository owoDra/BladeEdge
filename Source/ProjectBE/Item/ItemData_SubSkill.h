// Copyright (C) 2024 owoDra

#pragma once

#include "ItemData.h"

#include "ItemData_SubSkill.generated.h"


/**
 * キャラクターが使用可能なサブスキルを表すデータ
 */
UCLASS(meta = (DisplayName = "Sub Skill Data"))
class PROJECTBE_API UItemData_SubSkill : public UItemData
{
	GENERATED_BODY()
public:
	UItemData_SubSkill(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//
	// このデータを表す PrimaryAssetType 名
	//
	inline static const FName NAME_SubSkillDataType{ TEXTVIEW("SubSkill") };

};
