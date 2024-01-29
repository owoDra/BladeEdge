// Copyright (C) 2024 owoDra

#include "BEItemData_Weapon.h"

#include "GameplayTag/BETags_Equipment.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEItemData_Weapon)

/////////////////////////////////////////////////////////////////////////////////////////////////////

UBEItemData_Weapon::UBEItemData_Weapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ItemType = UBEItemData_Weapon::NAME_WeaponDataType;
}

FGameplayTag UBEItemData_Weapon::GetSlotTag() const
{
	return TAG_Equipment_Slot_Weapon;
}
