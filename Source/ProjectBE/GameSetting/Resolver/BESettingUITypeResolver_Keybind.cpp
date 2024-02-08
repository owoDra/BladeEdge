// Copyright (C) 2024 owoDra

#include "BESettingUITypeResolver_Keybind.h"

#include "GameplayTagContainer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BESettingUITypeResolver_Keybind)


UBESettingUITypeResolver_Keybind::UBESettingUITypeResolver_Keybind(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


void UBESettingUITypeResolver_Keybind::SetPrimary(FKey NewKey)
{
	if (ensure(InputSetting.IsValid()))
	{
		FMapPlayerKeyArgs NewArg;
		NewArg.NewKey = NewKey;
		NewArg.MappingName = GetDevName();
		NewArg.Slot = PrimarySlot;
		NewArg.bCreateMatchingSlotIfNeeded = true;
		NewArg.bDeferOnSettingsChangedBroadcast = false;

		FGameplayTagContainer FailureReason;
		InputSetting->MapPlayerKey(NewArg, FailureReason);

		if (FailureReason.IsEmpty())
		{
			NotifyPropertyValueChange(true);
		}
	}
}

void UBESettingUITypeResolver_Keybind::SetSecondary(FKey NewKey)
{
	if (ensure(InputSetting.IsValid()))
	{
		FMapPlayerKeyArgs NewArg;
		NewArg.NewKey = NewKey;
		NewArg.MappingName = GetDevName();
		NewArg.Slot = SecondarySlot;
		NewArg.bCreateMatchingSlotIfNeeded = true;
		NewArg.bDeferOnSettingsChangedBroadcast = false;

		FGameplayTagContainer FailureReason;
		InputSetting->MapPlayerKey(NewArg, FailureReason);

		if (FailureReason.IsEmpty())
		{
			NotifyPropertyValueChange(true);
		}
	}
}

void UBESettingUITypeResolver_Keybind::ResetBinds()
{
	if (ensure(InputSetting.IsValid()))
	{
		auto bChanged{ false };

		FMapPlayerKeyArgs NewArg;
		NewArg.MappingName = GetDevName();
		NewArg.Slot = PrimarySlot;
		NewArg.bCreateMatchingSlotIfNeeded = true;
		NewArg.bDeferOnSettingsChangedBroadcast = false;

		FGameplayTagContainer FailureReason;
		InputSetting->UnMapPlayerKey(NewArg, FailureReason);
		bChanged |= FailureReason.IsEmpty();

		NewArg.Slot = SecondarySlot;
		InputSetting->UnMapPlayerKey(NewArg, FailureReason);
		bChanged |= FailureReason.IsEmpty();

		if (bChanged)
		{
			NotifyPropertyValueChange(true);
		}
	}
}

void UBESettingUITypeResolver_Keybind::GetBinds(FKey& OutPrimary, FKey& OutSecondary) const
{
	if (ensure(InputSetting.IsValid()))
	{
		const auto* PrimaryMaping{ InputSetting->FindCurrentMappingForSlot(GetDevName(), PrimarySlot) };
		OutPrimary = PrimaryMaping ? PrimaryMaping->GetCurrentKey() : FKey();

		const auto* SecondaryMaping{ InputSetting->FindCurrentMappingForSlot(GetDevName(), SecondarySlot) };
		OutSecondary = SecondaryMaping ? SecondaryMaping->GetCurrentKey() : FKey();
	}
}
