// Copyright (C) 2024 owoDra

#pragma once

#include "Loadout/Type/BELoadoutRequest.h"
#include "Loadout/Type/BELoadout.h"

#include "BELoadoutRequestResolver.generated.h"



/**
 * Loadoutの変更リクエストを解決するためのデータ
 */
USTRUCT()
struct FBELoadoutRequestResolver
{
	GENERATED_BODY()
public:
	FBELoadoutRequestResolver() = default;

	FBELoadoutRequestResolver(const FBELoadoutRequest& InRequest);

protected:
	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<const UBEEquipmentItemData>> Requests;

	UPROPERTY()
	FName FighterSkinName{ NAME_None };
	
	UPROPERTY()
	FName WeaponSkinName{ NAME_None };

public:
	void ResolveRequest(FBELoadout& InOutLoadout);

protected:
	FName ResolveSkinName(const FGameplayTag& SlotTag);

};
