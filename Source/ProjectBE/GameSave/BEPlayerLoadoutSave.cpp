// Copyright (C) 2024 owoDra

#include "BEPlayerLoadoutSave.h"

#include "DeveloperSettings/BELoadoutDeveloperSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEPlayerLoadoutSave)


void UBEPlayerLoadoutSave::ResetToDefault()
{
	Super::ResetToDefault();

	FighterSkin.Empty();
	WeaponSkin.Empty();

	const auto* DevSettings{ GetDefault<UBELoadoutDeveloperSettings>() };

	EquipmentItems = DevSettings->DefaultEquipmentItems;
}
