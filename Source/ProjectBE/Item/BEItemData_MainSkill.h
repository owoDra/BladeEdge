// Copyright (C) 2024 owoDra

#pragma once

#include "Item/BEEquipmentItemData.h"

#include "BEItemData_MainSkill.generated.h"


/**
 * キャラクターが使用可能なメインスキルを表すデータ
 */
UCLASS(meta = (DisplayName = "Main Skill Data"))
class PROJECTBE_API UBEItemData_MainSkill : public UBEEquipmentItemData
{
	GENERATED_BODY()
public:
	UBEItemData_MainSkill(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//
	// このデータを表す PrimaryAssetType 名
	//
	inline static const FName NAME_MainSkillDataType{ TEXTVIEW("MainSkill") };

	virtual FGameplayTag GetSlotTag() const override;
};
