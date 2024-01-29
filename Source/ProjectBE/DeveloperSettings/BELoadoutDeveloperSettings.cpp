// Copyright (C) 2024 owoDra

#include "BELoadoutDeveloperSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BELoadoutDeveloperSettings)


UBELoadoutDeveloperSettings::UBELoadoutDeveloperSettings()
{
	CategoryName = TEXT("ProjectBE");
	SectionName = TEXT("Loadout");
}


FPrimaryAssetId UBELoadoutDeveloperSettings::GetEquipmentItemByTag(const FGameplayTag& SlotTag)
{
	return GetDefault<UBELoadoutDeveloperSettings>()->DefaultEquipmentItems.FindRef(SlotTag);
}
