// Copyright (C) 2024 owoDra

#pragma once

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

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<const UBEEquipmentItemData> FighterData{ nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<const UBEEquipmentItemData> WeaponData{ nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<const UBEEquipmentItemData> MainSkillData{ nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<const UBEEquipmentItemData> SubSkillData{ nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<const UBEEquipmentItemData> UltimateSkillData{ nullptr };

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
