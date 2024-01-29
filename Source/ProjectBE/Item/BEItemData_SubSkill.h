// Copyright (C) 2024 owoDra

#pragma once

#include "Item/BEEquipmentItemData.h"

#include "BEItemData_SubSkill.generated.h"


/**
 * キャラクターが使用可能なサブスキルを表すデータ
 */
UCLASS(meta = (DisplayName = "Sub Skill Data"))
class PROJECTBE_API UBEItemData_SubSkill : public UBEEquipmentItemData
{
	GENERATED_BODY()
public:
	UBEItemData_SubSkill(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//
	// このデータを表す PrimaryAssetType 名
	//
	inline static const FName NAME_SubSkillDataType{ TEXTVIEW("SubSkill") };

	virtual FGameplayTag GetSlotTag() const override;
};
