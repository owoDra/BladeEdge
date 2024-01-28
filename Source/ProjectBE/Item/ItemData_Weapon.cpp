// Copyright (C) 2024 owoDra

#include "ItemData_Weapon.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ItemData_Weapon)

/////////////////////////////////////////////////////////////////////////////////////////////////////

UItemData_Weapon::UItemData_Weapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ItemType = UItemData_Weapon::NAME_WeaponDataType;
}
