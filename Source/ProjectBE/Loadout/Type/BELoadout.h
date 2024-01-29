// Copyright (C) 2024 owoDra

#pragma once

#include "Net/Serialization/FastArraySerializer.h"

#include "Loadout/Type/BELoadoutRequest.h"

#include "BELoadout.generated.h"

class UBEEquipmentItemData;


/**
 * レプリケートされたロードアウト内のアイテムエントリーデータ
 */
USTRUCT()
struct FBELoadoutItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

public:
	FBELoadoutItem() = default;

	FBELoadoutItem(const UBEEquipmentItemData* InItemData, const FName& InSkinName)
		: ItemData(InItemData), SkinName(InSkinName)
	{}

public:
	//
	// 装備アイテムデータ
	//
	UPROPERTY()
	TObjectPtr<const UBEEquipmentItemData> ItemData{ nullptr };

	//
	// 装備アイテムに適用されているスキン名
	//
	UPROPERTY()
	FName SkinName{ NAME_None };

public:
	/**
	 * デバック用の文字列を返す
	 */
	FString GetDebugString() const;

	/**
	 * スロットタグを返す
	 */
	FGameplayTag GetSlotTag() const;

};


/**
 * レプリケートされたロードアウトデータ
 */
USTRUCT(BlueprintType)
struct FBELoadout : public FFastArraySerializer
{
	GENERATED_BODY()
public:
	FBELoadout() = default;

public:
	//
	// ロードアウト内のアイテムリスト
	//
	UPROPERTY()
	TArray<FBELoadoutItem> Entries;

	//
	// ロードアウト内のアイテムに素早くアクセスするためのインデクスマップ
	//
	UPROPERTY(NotReplicated)
	TMap<FGameplayTag, int32> IndexMap;

public:
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FBELoadoutItem, FBELoadout>(Entries, DeltaParms, *this);
	}

};

template<>
struct TStructOpsTypeTraits<FBELoadout> : public TStructOpsTypeTraitsBase2<FBELoadout>
{
	enum { WithNetDeltaSerializer = true };
};
