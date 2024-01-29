// Copyright (C) 2024 owoDra

#include "BEItemData_SubSkill.h"

#include "GameplayTag/BETags_Equipment.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEItemData_SubSkill)

/////////////////////////////////////////////////////////////////////////////////////////////////////

UBEItemData_SubSkill::UBEItemData_SubSkill(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ItemType = UBEItemData_SubSkill::NAME_SubSkillDataType;
}

FGameplayTag UBEItemData_SubSkill::GetSlotTag() const
{
	return TAG_Equipment_Slot_SubSkill;
}
