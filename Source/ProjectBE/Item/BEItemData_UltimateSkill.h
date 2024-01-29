// Copyright (C) 2024 owoDra

#pragma once

#include "Item/BEEquipmentItemData.h"

#include "BEItemData_UltimateSkill.generated.h"


/**
 * キャラクターが使用可能なアルティメットスキルを表すデータ
 */
UCLASS(meta = (DisplayName = "Ultimate Skill Data"))
class PROJECTBE_API UBEItemData_UltimateSkill : public UBEEquipmentItemData
{
	GENERATED_BODY()
public:
	UBEItemData_UltimateSkill(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//
	// このデータを表す PrimaryAssetType 名
	//
	inline static const FName NAME_UltimateSkillDataType{ TEXTVIEW("UltimateSkill") };

	virtual FGameplayTag GetSlotTag() const override;
};
