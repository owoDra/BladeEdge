// Copyright (C) 2024 owoDra

#include "BELoadoutRequestResolver.h"

#include "GameplayTag/BETags_Equipment.h"
#include "Item/BEEquipmentItemData.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BELoadoutRequestResolver)


FBELoadoutRequestResolver::FBELoadoutRequestResolver(const FBELoadoutRequest& InRequest)
{
	FighterSkinName = InRequest.FighterSkinName;
	WeaponSkinName = InRequest.WeaponSkinName;

	for (const auto& ItemData : InRequest.ItemDatas)
	{
		const auto SlotTag{ ItemData ? ItemData->GetSlotTag() : FGameplayTag::EmptyTag };

		if (SlotTag.IsValid())
		{
			Requests.Emplace(SlotTag, ItemData);
		}
	}
}


void FBELoadoutRequestResolver::ResolveRequest(FBELoadout& InOutLoadout)
{
	// まずエントリー内の要素をイテレートする

	for (auto It{ InOutLoadout.Entries.CreateIterator() }; It; ++It)
	{
		auto& Entry{ *It };
		const auto& Tag{ Entry.GetSlotTag() };

		// リクエストにあるスロットタグなら更新する

		if (auto* Found{ Requests.Find(Tag) })
		{
			Entry.ItemData = *Found;
			Entry.SkinName = ResolveSkinName(Tag);
			InOutLoadout.MarkItemDirty(Entry);

			// このタイミングで適用済みのリクエストを削除する

			Requests.Remove(Tag);
		}

		// リクエストにないスロットタグだった場合は削除する

		else
		{
			It.RemoveCurrent();
			InOutLoadout.MarkArrayDirty();
		}
	}

	// 残ったリクエストを新規追加分として追加する

	for (const auto& Request : Requests)
	{
		const auto& Tag{ Request.Key };
		const auto& Data{ Request.Value };

		auto& NewEntry{ InOutLoadout.Entries.Emplace_GetRef(Data, ResolveSkinName(Tag)) };
		InOutLoadout.MarkItemDirty(NewEntry);
	}
}

FName FBELoadoutRequestResolver::ResolveSkinName(const FGameplayTag& SlotTag)
{
	return (SlotTag == TAG_Equipment_Slot_Fighter) ? FighterSkinName :
			(SlotTag == TAG_Equipment_Slot_Weapon) ? WeaponSkinName : FName();
}
