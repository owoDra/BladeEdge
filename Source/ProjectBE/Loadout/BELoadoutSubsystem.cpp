// Copyright (C) 2024 owoDra

#include "BELoadoutSubsystem.h"

#include "GameSave/BEPlayerLoadoutSave.h"
#include "GameplayTag/BETags_LoadingType.h"
#include "DeveloperSettings/BELoadoutDeveloperSettings.h"
#include "Item/BEEquipmentItemData.h"
#include "ProjectBELogs.h"

// Game Framework Core
#include "AssetManager/GFCAssetManager.h"

// Game Save Core
#include "PlayerSaveSubsystem.h"

// Game Loading Core
#include "LoadingScreenSubsystem.h"
#include "GameplayTag/GCLoadingTags_LoadingType.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BELoadoutSubsystem)


// Initialization

void UBELoadoutSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	auto* SaveSubsystem{ Collection.InitializeDependency<UPlayerSaveSubsystem>() };

	InitializeLoadout(SaveSubsystem);
}

void UBELoadoutSubsystem::InitializeLoadout(UPlayerSaveSubsystem* SaveSubsystem)
{
	// ロード画面を表示

	HandleShowLoadingLoadoutScreen();

	// ロードアウトセーブを非同期ロード開始

	auto NewDelegate
	{
		FPlayerSaveEventDelegate::CreateUObject(this, &ThisClass::HandleLoadoutSaveLoaded)
	};

	auto bSuccess
	{
		SaveSubsystem->AsyncLoadPlayerSave(
			UBEPlayerLoadoutSave::StaticClass()
			, /* SlotName	= */ FString()
			, /* ForceLoad	= */ true
			, /* Callback	= */ NewDelegate)
	};

	if (!bSuccess)
	{
		HandleLoadoutSaveLoaded(nullptr, false);
	}
}

void UBELoadoutSubsystem::HandleLoadoutSaveLoaded(UPlayerSave* Save, bool bSuccess)
{
	EquipmentItems.Reset();

	// ロードされたセーブからデータを取得

	auto* LoadoutSave{ Cast<UBEPlayerLoadoutSave>(Save) };

	if (ensure(LoadoutSave))
	{
		// 保存されていた装備情報をロードアウトに設定

		for (const auto& KVP : LoadoutSave->EquipmentItems)
		{
			const auto& SlotTag{ KVP.Key };
			const auto& AssetId{ KVP.Value };

			SetLoadoutItem(SlotTag, AssetId);
		}

		// 保存されていたスキン情報をロードアウトに設定

		FighterSkin = LoadoutSave->FighterSkin;
		WeaponSkin = LoadoutSave->WeaponSkin;
	}

	// ロード画面を非表示

	HandleHideLoadingLoadoutScreen();
}


// Loading Screen

void UBELoadoutSubsystem::HandleShowLoadingLoadoutScreen()
{
	auto* GameInstance{ GetLocalPlayer()->GetGameInstance() };
	auto* LSSubsystem{ UGameInstance::GetSubsystem<ULoadingScreenSubsystem>(GameInstance) };

	if (LSSubsystem)
	{
		LSSubsystem->AddLoadingProcess(
			UBELoadoutSubsystem::NAME_LoadoutLoading
			, TAG_LoadingType_Fullscreen
			, FText(NSLOCTEXT("LoadingScreen", "LoadoutLoadingReason", "Loading Loadout From Save")));
	}
}

void UBELoadoutSubsystem::HandleHideLoadingLoadoutScreen()
{
	auto* GameInstance{ GetLocalPlayer()->GetGameInstance() };
	auto* LSSubsystem{ UGameInstance::GetSubsystem<ULoadingScreenSubsystem>(GameInstance) };

	if (LSSubsystem)
	{
		LSSubsystem->RemoveLoadingProcess(UBELoadoutSubsystem::NAME_LoadoutLoading);
	}
}

void UBELoadoutSubsystem::HandleShowSavingLoadoutIndicator()
{
	auto* GameInstance{ GetLocalPlayer()->GetGameInstance() };
	auto* LSSubsystem{ UGameInstance::GetSubsystem<ULoadingScreenSubsystem>(GameInstance) };

	if (LSSubsystem)
	{
		LSSubsystem->AddLoadingProcess(
			UBELoadoutSubsystem::NAME_LoadoutSaving
			, TAG_LoadingType_AsyncIndicator
			, FText(NSLOCTEXT("LoadingScreen", "LoadoutSavingReason", "Saving Loadout Changes")));
	}
}

void UBELoadoutSubsystem::HandleHideSavingLoadoutIndicator()
{
	auto* GameInstance{ GetLocalPlayer()->GetGameInstance() };
	auto* LSSubsystem{ UGameInstance::GetSubsystem<ULoadingScreenSubsystem>(GameInstance) };

	if (LSSubsystem)
	{
		LSSubsystem->RemoveLoadingProcess(UBELoadoutSubsystem::NAME_LoadoutSaving);
	}
}


// Skin

void UBELoadoutSubsystem::SetSkin(EBESkinAccessorType Type, FPrimaryAssetId AssetId, FName SkinName)
{
	auto& SelectedMap{ (Type == EBESkinAccessorType::Fighter) ? FighterSkin : WeaponSkin };

	SelectedMap.Add(AssetId, SkinName);
}

FName UBELoadoutSubsystem::GetSkin(EBESkinAccessorType Type, FPrimaryAssetId AssetId) const
{
	auto& SelectedMap{ (Type == EBESkinAccessorType::Fighter) ? FighterSkin : WeaponSkin };

	return SelectedMap.FindRef(AssetId);
}


// Equipment Item Data

void UBELoadoutSubsystem::SetLoadoutItem(FGameplayTag SlotTag, FPrimaryAssetId AssetId)
{
	if (SlotTag.IsValid())
	{
		// 初めに引数のアセットIdからデータを取得

		if (auto* Data_FromArg{ ProcessEquipmentItemData(SlotTag, AssetId) })
		{
			EquipmentItems.Emplace(SlotTag, Data_FromArg);
			return;
		}

		// 引数のアセットIdでは取得できなかった場合はデフォルト設定から設定する

		UE_LOG(LogBE_Loadout, Warning, TEXT("SetLoadoutItem(): Cannot get Data from arg(Tag: %s, Id: %s)"), *SlotTag.GetTagName().ToString(), *AssetId.ToString());

		auto AssetId_Default{ UBELoadoutDeveloperSettings::GetEquipmentItemByTag(SlotTag) };
		if (AssetId_Default.IsValid())
		{
			if (auto* Data_Default{ ProcessEquipmentItemData(SlotTag, AssetId_Default) })
			{
				EquipmentItems.Emplace(SlotTag, Data_Default);
				return;
			}
		}

		// デフォルトからも取得できなかった場合はエラーを出す

		UE_LOG(LogBE_Loadout, Error, TEXT("SetLoadoutItem(): Cannot get Data from defaul(Tag: %s, Id: %s)"), *SlotTag.GetTagName().ToString(), *AssetId_Default.ToString());
	}
	else
	{
		UE_LOG(LogBE_Loadout, Error, TEXT("SetLoadoutItem(): Invalid Slota Tag"));
	}
}

FPrimaryAssetId UBELoadoutSubsystem::GetLoadoutItemAssetId(FGameplayTag SlotTag)
{
	auto ItemData{ GetLoadoutItemData(SlotTag) };
	return ItemData ? ItemData->GetPrimaryAssetId() : FPrimaryAssetId();
}

const UBEEquipmentItemData* UBELoadoutSubsystem::GetLoadoutItemData(FGameplayTag SlotTag)
{
	return EquipmentItems.FindRef(SlotTag);
}

const UBEEquipmentItemData* UBELoadoutSubsystem::ProcessEquipmentItemData(const FGameplayTag& SlotTag, const FPrimaryAssetId& AssetId) const
{
	if (const auto* Data{ UGFCAssetManager::GetAsset<UBEEquipmentItemData>(AssetId) })
	{
		if (Data->GetSlotTag() == SlotTag)
		{
			return Data;
		}
	}

	return nullptr;
}


// Save

bool UBELoadoutSubsystem::SaveLoadout()
{
	auto* LocalPlayer{ GetLocalPlayer() };
	auto* SaveSubsystem{ LocalPlayer ? LocalPlayer->GetSubsystem<UPlayerSaveSubsystem>() : nullptr };
	auto* LoadoutSave{ SaveSubsystem ? SaveSubsystem->GetSave<UBEPlayerLoadoutSave>() : nullptr };

	if (LoadoutSave)
	{
		// 現在のロードアウトの値をセーブオブジェクトに書き込み

		LoadoutSave->FighterSkin = FighterSkin;
		LoadoutSave->WeaponSkin = WeaponSkin;

		auto& SavedEquipmentItems{ LoadoutSave->EquipmentItems };
		SavedEquipmentItems.Reset();

		for (const auto& KVP : EquipmentItems)
		{
			const auto& Tag{ KVP.Key };
			const auto AssetId{ KVP.Value ? KVP.Value->GetPrimaryAssetId() : FPrimaryAssetId() };

			SavedEquipmentItems.Emplace(Tag, AssetId);
		}

		// Delegate を作成

		auto NewDelegate
		{
			FPlayerSaveEventDelegate::CreateUObject(this, &ThisClass::HandleLoadoutSaved)
		};

		if (SaveSubsystem->AsyncSaveGameToSlot(UBEPlayerLoadoutSave::StaticClass(), FString(), NewDelegate))
		{
			// ローディングインジケーターを表示

			HandleShowSavingLoadoutIndicator();

			return true;
		}
	}

	return false;
}

void UBELoadoutSubsystem::HandleLoadoutSaved(UPlayerSave* Save, bool bSuccess)
{
	HandleHideSavingLoadoutIndicator();
}


// Request

FBELoadoutRequest UBELoadoutSubsystem::CreateRequest() const
{
	TArray<TObjectPtr<const UBEEquipmentItemData>> Items;
	EquipmentItems.GenerateValueArray(Items);
	return FBELoadoutRequest(Items);
}
