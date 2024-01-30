// Copyright (C) 2024 owoDra

#include "BELoadoutRequest.h"

#include "GameplayTag/BETags_Equipment.h"
#include "Item/BEEquipmentItemData.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BELoadoutRequest)


bool FBELoadoutRequest::IsValid() const
{
	return FighterData && WeaponData && MainSkillData && SubSkillData && UltimateSkillData;
}

FString FBELoadoutRequest::GetDebugString() const
{
	return FString::Printf(TEXT("[%s, %s, %s, %s, %s]")
		, *GetNameSafe(FighterData)
		, *GetNameSafe(WeaponData)
		, *GetNameSafe(MainSkillData)
		, *GetNameSafe(SubSkillData)
		, *GetNameSafe(UltimateSkillData));
}
