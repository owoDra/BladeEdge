// Copyright (C) 2024 owoDra

#include "BEItemData_MainSkill.h"

#include "GameplayTag/BETags_Equipment.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEItemData_MainSkill)

/////////////////////////////////////////////////////////////////////////////////////////////////////

UBEItemData_MainSkill::UBEItemData_MainSkill(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ItemType = UBEItemData_MainSkill::NAME_MainSkillDataType;
}

FGameplayTag UBEItemData_MainSkill::GetSlotTag() const
{
	return TAG_Equipment_Slot_MainSkill;
}
