// Copyright (C) 2024 owoDra

#include "BEItemData_UltimateSkill.h"

#include "GameplayTag/BETags_Equipment.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEItemData_UltimateSkill)

/////////////////////////////////////////////////////////////////////////////////////////////////////

UBEItemData_UltimateSkill::UBEItemData_UltimateSkill(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ItemType = UBEItemData_UltimateSkill::NAME_UltimateSkillDataType;
}

FGameplayTag UBEItemData_UltimateSkill::GetSlotTag() const
{
	return TAG_Equipment_Slot_UltimateSkill;
}
