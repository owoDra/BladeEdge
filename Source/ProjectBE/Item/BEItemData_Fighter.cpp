// Copyright (C) 2024 owoDra

#include "BEItemData_Fighter.h"

#include "GameplayTag/BETags_Equipment.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEItemData_Fighter)

/////////////////////////////////////////////////////////////////////////////////////////////////////

UBEItemData_Fighter::UBEItemData_Fighter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ItemType = UBEItemData_Fighter::NAME_FighterDataType;
}

FGameplayTag UBEItemData_Fighter::GetSlotTag() const
{
	return TAG_Equipment_Slot_Fighter;
}
