// Copyright (C) 2024 owoDra

#include "BELoadout.h"

#include "Item/BEEquipmentItemData.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BELoadout)


///////////////////////////////////////////////////////////////////
// FBELoadoutItem

FString FBELoadoutItem::GetDebugString() const
{
	return FString::Printf(TEXT("Slot: %s, Data: %s")
		, *GetSlotTag().ToString()
		, *GetNameSafe(ItemData));
}

FGameplayTag FBELoadoutItem::GetSlotTag() const
{
	return ItemData ? ItemData->GetSlotTag() : FGameplayTag::EmptyTag;
}
