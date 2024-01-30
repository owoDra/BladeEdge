// Copyright (C) 2024 owoDra

#include "BELoadoutRequest.h"

#include "Item/BEEquipmentItemData.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BELoadoutRequest)


bool FBELoadoutRequest::IsValid() const
{
	return !ItemDatas.IsEmpty();
}

FString FBELoadoutRequest::GetDebugString() const
{
	auto Result{ FString::Printf(TEXT("FighterSkin: %s, WeaponSkin: %s "), *FighterSkinName.ToString(), *WeaponSkinName.ToString()) };

	for (const auto& Item : ItemDatas)
	{
		Result = FString::Printf(TEXT("%s[%s]"), *Result, *GetNameSafe(Item));
	}

	return Result;
}
