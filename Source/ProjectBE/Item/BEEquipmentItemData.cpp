// Copyright (C) 2024 owoDra

#include "BEEquipmentItemData.h"

#include "Item/ItemInfo_Equipment.h"
#include "Info/ItemInfo_UserFacing.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEEquipmentItemData)

/////////////////////////////////////////////////////////////////////////////////////////////////////

UBEEquipmentItemData::UBEEquipmentItemData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	auto* UserFacingInfo{ ObjectInitializer.CreateDefaultSubobject<UItemInfo_UserFacing>(this, "UserFacingInfo") };
	Infomations.Emplace(UserFacingInfo);

	auto* EquipmentInfo{ ObjectInitializer.CreateDefaultSubobject<UItemInfo_Equipment>(this, "EquipmentInfo")};
	Infomations.Emplace(EquipmentInfo);
}
