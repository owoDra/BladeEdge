// Copyright (C) 2024 owoDra

#include "BELoadoutSubsystem.h"

#include "GameSave/BEPlayerLoadoutSave.h"
#include "GameplayTag/BETags_LoadingType.h"
#include "DeveloperSettings/BELoadoutDeveloperSettings.h"
#include "Item/BEEquipmentItemData.h"
#include "Loadout/BELoadoutComponent.h"
#include "ProjectBELogs.h"

// Game Framework Core
#include "AssetManager/GFCAssetManager.h"

// Game Save Core
#include "PlayerSaveSubsystem.h"

// Game Loading Core
#include "LoadingScreenSubsystem.h"
#include "GameplayTag/GCLoadingTags_LoadingType.h"

// Engine Feature
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"

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

	// デフォルト値を設定する

	const auto* DevSettings{ GetDefault<UBELoadoutDeveloperSettings>() };
	SetFighterData(DevSettings->Fighter);
	SetWeaponData(DevSettings->Weapon);
	SetMainSkillData(DevSettings->MainSkill);
	SetSubSkillData(DevSettings->SubSkill);
	SetUltimateSkillData(DevSettings->UltimateSkill);

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
		// ロード画面を非表示

		HandleHideLoadingLoadoutScreen();
	}
}

void UBELoadoutSubsystem::HandleLoadoutSaveLoaded(UPlayerSave* Save, bool bSuccess)
{
	// ロードされたセーブからデータを取得

	auto* LoadoutSave{ Cast<UBEPlayerLoadoutSave>(Save) };

	if (ensure(LoadoutSave))
	{
		// 保存されていた装備情報をロードアウトに設定

		SetFighterData(LoadoutSave->Fighter);
		SetWeaponData(LoadoutSave->Weapon);
		SetMainSkillData(LoadoutSave->MainSkill);
		SetSubSkillData(LoadoutSave->SubSkill);
		SetUltimateSkillData(LoadoutSave->UltimateSkill);
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


// Equipment Item Data

void UBELoadoutSubsystem::ProcessItemDataFromId(FPrimaryAssetId AssetId, TObjectPtr<const UBEEquipmentItemData>& OutItemData)
{
	if (AssetId.IsValid())
	{
		if (const auto* Data{ UGFCAssetManager::GetAsset<UBEEquipmentItemData>(AssetId, false) })
		{
			OutItemData = Data;
		}
		else
		{
			UE_LOG(LogBE_Loadout, Error, TEXT("Item data could not be retrieved from AssetId(%s)"), *AssetId.ToString());
		}
	}
	else
	{
		UE_LOG(LogBE_Loadout, Error, TEXT("Tried to set item data with invalid AssetId"));
	}
}

FPrimaryAssetId UBELoadoutSubsystem::GetItemDataFromId(const UBEEquipmentItemData* ItemData) const
{
	return ItemData ? ItemData->GetPrimaryAssetId() : FPrimaryAssetId();
}


// Save

bool UBELoadoutSubsystem::SaveLoadout()
{
	auto* LocalPlayer{ GetLocalPlayer() };
	auto* SaveSubsystem{ LocalPlayer ? LocalPlayer->GetSubsystem<UPlayerSaveSubsystem>() : nullptr };
	auto* LoadoutSave{ SaveSubsystem ? SaveSubsystem->GetSave<UBEPlayerLoadoutSave>() : nullptr };

	if (LoadoutSave)
	{
		// ローディングインジケーターを表示

		HandleShowSavingLoadoutIndicator();

		// 現在のロードアウトの値をセーブオブジェクトに書き込み

		LoadoutSave->Fighter		= GetFighterDataId();
		LoadoutSave->Weapon			= GetWeaponDataId();
		LoadoutSave->MainSkill		= GetMainSkillDataId();
		LoadoutSave->SubSkill		= GetSubSkillDataId();
		LoadoutSave->UltimateSkill	= GetUltimateSkillDataId();


		// Delegate を作成

		auto NewDelegate
		{
			FPlayerSaveEventDelegate::CreateUObject(this, &ThisClass::HandleLoadoutSaved)
		};

		if (!SaveSubsystem->AsyncSaveGameToSlot(UBEPlayerLoadoutSave::StaticClass(), FString(), NewDelegate))
		{
			HandleHideSavingLoadoutIndicator();
			return false;
		}

		return true;
	}

	return false;
}

void UBELoadoutSubsystem::HandleLoadoutSaved(UPlayerSave* Save, bool bSuccess)
{
	HandleHideSavingLoadoutIndicator();
}


// Request

bool UBELoadoutSubsystem::SendLoadoutRequest() const
{
	auto* LocalPlayer{ GetLocalPlayer() };
	auto* PlayerController{ LocalPlayer ? LocalPlayer->GetPlayerController(GetWorld()) : nullptr};
	auto* PlayerState{ PlayerController ? PlayerController->GetPlayerState<APlayerState>() : nullptr };

	if (auto* LoadoutComponent{ UBELoadoutComponent::FindBELoadoutComponent(PlayerState) })
	{
		FBELoadoutRequest NewRequest;
		NewRequest.FighterData			= FighterData;
		NewRequest.WeaponData			= WeaponData;
		NewRequest.MainSkillData		= MainSkillData;
		NewRequest.SubSkillData			= SubSkillData;
		NewRequest.UltimateSkillData	= UltimateSkillData;

		LoadoutComponent->LoadoutRequest(NewRequest);
		return true;
	}

	return false;
}
