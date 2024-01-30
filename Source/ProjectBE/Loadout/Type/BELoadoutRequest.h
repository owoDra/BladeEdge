﻿// Copyright (C) 2024 owoDra

#pragma once

#include "GameplayTagContainer.h"

#include "BELoadoutRequest.generated.h"

class UBEEquipmentItemData;


/**
 * クライアントからサーバーへロードアウトの更新を知らせるのに使用するデータ
 */
USTRUCT(BlueprintType)
struct FBELoadoutRequest
{
	GENERATED_BODY()
public:
	FBELoadoutRequest() = default;

	FBELoadoutRequest(const TArray<const UBEEquipmentItemData*>& InItems)
		: ItemDatas(InItems)
	{}

public:
	UPROPERTY()
	TArray<TObjectPtr<const UBEEquipmentItemData>> ItemDatas;

	UPROPERTY()
	FName FighterSkinName{ NAME_None };
	
	UPROPERTY()
	FName WeaponSkinName{ NAME_None };

public:
	/**
	 * このリクエストが有効かを返す
	 */
	bool IsValid() const;

	/**
	 * デバッグ用の文字列を返す
	 */
	FString GetDebugString() const;
};