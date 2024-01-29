// Copyright (C) 2024 owoDra

#pragma once

#include "ItemData.h"

#include "GameplayTagContainer.h"

#include "BEEquipmentItemData.generated.h"


/**
 * このプロジェクトの装備アイテムのベースクラス
 */
UCLASS(Abstract)
class PROJECTBE_API UBEEquipmentItemData : public UItemData
{
	GENERATED_BODY()
public:
	UBEEquipmentItemData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * この装備アイテムに該当する装備スロットを返す
	 */
	virtual FGameplayTag GetSlotTag() const PURE_VIRTUAL(, return FGameplayTag::EmptyTag;);
};
