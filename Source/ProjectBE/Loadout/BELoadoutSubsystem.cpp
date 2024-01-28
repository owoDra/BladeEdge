// Copyright (C) 2024 owoDra

#include "BELoadoutSubsystem.h"

#include "GameSave/BEPlayerLoadoutSave.h"
#include "DeveloperSettings/BELoadoutDeveloperSettings.h"
#include "System/BEAssetManager.h"
#include "ProjectBELogs.h"

// Game Save Core
#include "PlayerSaveSubsystem.h"

// Game Item Core
#include "ItemData.h"

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
	const auto* LoadoutSave{ SaveSubsystem->GetSave<UBEPlayerLoadoutSave>() };
	check(LoadoutSave);

	SetFighterData(LoadoutSave->Fighter);
	SetWeaponData(LoadoutSave->Weapon);
	SetMainSkillData(LoadoutSave->MainSkill);
	SetSubSkillData(LoadoutSave->SubSkill);
	SetUltimateSkillData(LoadoutSave->UltimateSkill);
}


// Skin

void UBELoadoutSubsystem::SetFighterSkin(FPrimaryAssetId AssetId, FName FighterSkinName)
{
	if (auto* LoadoutSave{ GetSave() })
	{
		LoadoutSave->FighterSkin.Add(AssetId, FighterSkinName);
	}
}

FName UBELoadoutSubsystem::GetFighterSkin(FPrimaryAssetId AssetId) const
{
	if (auto* LoadoutSave{ GetSave() })
	{
		return LoadoutSave->FighterSkin.FindRef(AssetId);
	}

	return FName();
}

void UBELoadoutSubsystem::SetWeaponSkin(FPrimaryAssetId AssetId, FName WeaponSkinName)
{
	if (auto* LoadoutSave{ GetSave() })
	{
		LoadoutSave->WeaponSkin.Add(AssetId, WeaponSkinName);
	}
}

FName UBELoadoutSubsystem::GetWeaponSkin(FPrimaryAssetId AssetId) const
{
	if (auto* LoadoutSave{ GetSave() })
	{
		return LoadoutSave->WeaponSkin.FindRef(AssetId);
	}

	return FName();
}


// Fighter Data

void UBELoadoutSubsystem::SetFighterData(FPrimaryAssetId AssetId)
{
	// Get asset from argument PrimaryAssetId

	if (auto* DataByArg{ GetItemDataFromPrimaryAssetId(AssetId) })
	{
		SetFighterDataInternal(AssetId, DataByArg);
		return;
	}

	// Get asset from PrimaryAssetId in DeveSettings

	UE_LOG(LogBE_Loadout, Warning, TEXT("SetFighterData(): Could not find a valid asset from Argument's Id(%s)"), *AssetId.ToString());
	UE_LOG(LogBE_Loadout, Warning, TEXT("Try get asset from PrimaryAssetId in DeveSettings"));

	auto AssetIdFromDevSetting{ GetDefault<UBELoadoutDeveloperSettings>()->DefaultFighter };
	if (auto* DataByDevSetting{ GetItemDataFromPrimaryAssetId(AssetIdFromDevSetting) })
	{
		SetFighterDataInternal(AssetIdFromDevSetting, DataByDevSetting);
		return;
	}

	// Error if asset not found
	
	UE_LOG(LogBE_Loadout, Error, TEXT("SetFighterData(): Could not find a valid asset from either Argument's Id(%s) or DevSetting's Id(%s)."), *AssetId.ToString(), *AssetIdFromDevSetting.ToString());
}

const UItemData* UBELoadoutSubsystem::GetFighterData() const
{
	return Fighter;
}

FPrimaryAssetId UBELoadoutSubsystem::GetFighterDataId() const
{
	return Fighter ? Fighter->GetPrimaryAssetId() : FPrimaryAssetId();
}

void UBELoadoutSubsystem::SetFighterDataInternal(FPrimaryAssetId AssetId, const UItemData* Data)
{
	if (Data && (Fighter != Data))
	{
		Fighter = Data;

		// Set to save

		if (auto* LoadoutSave{ GetSave() })
		{
			LoadoutSave->Fighter = AssetId;
		}
	}
}


// Weapon

void UBELoadoutSubsystem::SetWeaponData(FPrimaryAssetId AssetId)
{
	// Get asset from argument PrimaryAssetId

	if (auto* DataByArg{ GetItemDataFromPrimaryAssetId(AssetId) })
	{
		SetWeaponDataInternal(AssetId, DataByArg);
		return;
	}

	// Get asset from PrimaryAssetId in DeveSettings

	UE_LOG(LogBE_Loadout, Warning, TEXT("SetWeaponData(): Could not find a valid asset from Argument's Id(%s)"), *AssetId.ToString());
	UE_LOG(LogBE_Loadout, Warning, TEXT("Try get asset from PrimaryAssetId in DeveSettings"));

	auto AssetIdFromDevSetting{ GetDefault<UBELoadoutDeveloperSettings>()->DefaultWeapon };
	if (auto* DataByDevSetting{ GetItemDataFromPrimaryAssetId(AssetIdFromDevSetting) })
	{
		SetWeaponDataInternal(AssetIdFromDevSetting, DataByDevSetting);
		return;
	}

	// Error if asset not found

	UE_LOG(LogBE_Loadout, Error, TEXT("SetWeaponData(): Could not find a valid asset from either Argument's Id(%s) or DevSetting's Id(%s)."), *AssetId.ToString(), *AssetIdFromDevSetting.ToString());
}

const UItemData* UBELoadoutSubsystem::GetWeaponData() const
{
	return Weapon;
}

FPrimaryAssetId UBELoadoutSubsystem::GetWeaponDataId() const
{
	return Weapon ? Weapon->GetPrimaryAssetId() : FPrimaryAssetId();
}

void UBELoadoutSubsystem::SetWeaponDataInternal(FPrimaryAssetId AssetId, const UItemData* Data)
{
	if (Data && (Weapon != Data))
	{
		Weapon = Data;

		// Set to save

		if (auto* LoadoutSave{ GetSave() })
		{
			LoadoutSave->Weapon = AssetId;
		}
	}
}


// Main Skill

void UBELoadoutSubsystem::SetMainSkillData(FPrimaryAssetId AssetId)
{
	// Get asset from argument PrimaryAssetId

	if (auto* DataByArg{ GetItemDataFromPrimaryAssetId(AssetId) })
	{
		SetMainSkillDataInternal(AssetId, DataByArg);
		return;
	}

	// Get asset from PrimaryAssetId in DeveSettings

	UE_LOG(LogBE_Loadout, Warning, TEXT("SetMainSkillData(): Could not find a valid asset from Argument's Id(%s)"), *AssetId.ToString());
	UE_LOG(LogBE_Loadout, Warning, TEXT("Try get asset from PrimaryAssetId in DeveSettings"));

	auto AssetIdFromDevSetting{ GetDefault<UBELoadoutDeveloperSettings>()->DefaultMainSkill };
	if (auto* DataByDevSetting{ GetItemDataFromPrimaryAssetId(AssetIdFromDevSetting) })
	{
		SetMainSkillDataInternal(AssetIdFromDevSetting, DataByDevSetting);
		return;
	}

	// Error if asset not found

	UE_LOG(LogBE_Loadout, Error, TEXT("SetMainSkillData(): Could not find a valid asset from either Argument's Id(%s) or DevSetting's Id(%s)."), *AssetId.ToString(), *AssetIdFromDevSetting.ToString());
}

const UItemData* UBELoadoutSubsystem::GetMainSkillData() const
{
	return MainSkill;
}

FPrimaryAssetId UBELoadoutSubsystem::GetMainSkillDataId() const
{
	return MainSkill ? MainSkill->GetPrimaryAssetId() : FPrimaryAssetId();
}

void UBELoadoutSubsystem::SetMainSkillDataInternal(FPrimaryAssetId AssetId, const UItemData* Data)
{
	if (Data && (MainSkill != Data))
	{
		MainSkill = Data;

		// Set to save

		if (auto* LoadoutSave{ GetSave() })
		{
			LoadoutSave->MainSkill = AssetId;
		}
	}
}


// Sub Skill

void UBELoadoutSubsystem::SetSubSkillData(FPrimaryAssetId AssetId)
{
	// Get asset from argument PrimaryAssetId

	if (auto* DataByArg{ GetItemDataFromPrimaryAssetId(AssetId) })
	{
		SetSubSkillDataInternal(AssetId, DataByArg);
		return;
	}

	// Get asset from PrimaryAssetId in DeveSettings

	UE_LOG(LogBE_Loadout, Warning, TEXT("SetSubSKillData(): Could not find a valid asset from Argument's Id(%s)"), *AssetId.ToString());
	UE_LOG(LogBE_Loadout, Warning, TEXT("Try get asset from PrimaryAssetId in DeveSettings"));

	auto AssetIdFromDevSetting{ GetDefault<UBELoadoutDeveloperSettings>()->DefaultSubSkill };
	if (auto* DataByDevSetting{ GetItemDataFromPrimaryAssetId(AssetIdFromDevSetting) })
	{
		SetSubSkillDataInternal(AssetIdFromDevSetting, DataByDevSetting);
		return;
	}

	// Error if asset not found

	UE_LOG(LogBE_Loadout, Error, TEXT("SetSubSKillData(): Could not find a valid asset from either Argument's Id(%s) or DevSetting's Id(%s)."), *AssetId.ToString(), *AssetIdFromDevSetting.ToString());
}

const UItemData* UBELoadoutSubsystem::GetSubSkillData() const
{
	return SubSkill;
}

FPrimaryAssetId UBELoadoutSubsystem::GetSubSkillDataId() const
{
	return SubSkill ? SubSkill->GetPrimaryAssetId() : FPrimaryAssetId();
}

void UBELoadoutSubsystem::SetSubSkillDataInternal(FPrimaryAssetId AssetId, const UItemData* Data)
{
	if (Data && (SubSkill != Data))
	{
		SubSkill = Data;

		// Set to save

		if (auto* LoadoutSave{ GetSave() })
		{
			LoadoutSave->SubSkill = AssetId;
		}
	}
}


// Ultimate Skill

void UBELoadoutSubsystem::SetUltimateSkillData(FPrimaryAssetId AssetId)
{
	// Get asset from argument PrimaryAssetId

	if (auto* DataByArg{ GetItemDataFromPrimaryAssetId(AssetId) })
	{
		SetUltimateSkillDataInternal(AssetId, DataByArg);
		return;
	}

	// Get asset from PrimaryAssetId in DeveSettings

	UE_LOG(LogBE_Loadout, Warning, TEXT("SetUltimateSkillData(): Could not find a valid asset from Argument's Id(%s)"), *AssetId.ToString());
	UE_LOG(LogBE_Loadout, Warning, TEXT("Try get asset from PrimaryAssetId in DeveSettings"));

	auto AssetIdFromDevSetting{ GetDefault<UBELoadoutDeveloperSettings>()->DefaultUltimateSkill };
	if (auto* DataByDevSetting{ GetItemDataFromPrimaryAssetId(AssetIdFromDevSetting) })
	{
		SetUltimateSkillDataInternal(AssetIdFromDevSetting, DataByDevSetting);
		return;
	}

	// Error if asset not found

	UE_LOG(LogBE_Loadout, Error, TEXT("SetUltimateSkillData(): Could not find a valid asset from either Argument's Id(%s) or DevSetting's Id(%s)."), *AssetId.ToString(), *AssetIdFromDevSetting.ToString());
}

const UItemData* UBELoadoutSubsystem::GetUltimateSkillData() const
{
	return UltimateSkill;
}

FPrimaryAssetId UBELoadoutSubsystem::GetUltimateSkillDataId() const
{
	return UltimateSkill ? UltimateSkill->GetPrimaryAssetId() : FPrimaryAssetId();
}

void UBELoadoutSubsystem::SetUltimateSkillDataInternal(FPrimaryAssetId AssetId, const UItemData* Data)
{
	if (Data && (UltimateSkill != Data))
	{
		UltimateSkill = Data;

		// Set to save

		if (auto* LoadoutSave{ GetSave() })
		{
			LoadoutSave->UltimateSkill = AssetId;
		}
	}
}


// Utilities

UBEPlayerLoadoutSave* UBELoadoutSubsystem::GetSave() const
{
	auto* LocalPlayer{ GetLocalPlayer() };
	auto* SaveSubsystem{ LocalPlayer ? LocalPlayer->GetSubsystem<UPlayerSaveSubsystem>() : nullptr };
	return SaveSubsystem ? SaveSubsystem->GetSave<UBEPlayerLoadoutSave>() : nullptr;
}

const UItemData* UBELoadoutSubsystem::GetItemDataFromPrimaryAssetId(FPrimaryAssetId AssetId) const
{
	if (!AssetId.IsValid())
	{
		UE_LOG(LogBE_Loadout, Error, TEXT("GetItemDataFromPrimaryAssetId(): Invalid AssetId passed"));
		return nullptr;
	}

	if (auto* Data{ UBEAssetManager::GetAsset<UItemData>(AssetId) })
	{
		return Data;
	}

	UE_LOG(LogBE_Loadout, Error, TEXT("GetItemDataFromPrimaryAssetId(): Cannot get Data from PrimaryAssetId(%s)"), *AssetId.ToString());
	return nullptr;
}

bool UBELoadoutSubsystem::SaveLoadout()
{
	auto* LocalPlayer{ GetLocalPlayer() };
	
	if (auto* SaveSubsystem{ LocalPlayer ? LocalPlayer->GetSubsystem<UPlayerSaveSubsystem>() : nullptr })
	{
		return SaveSubsystem->AsyncSaveGameToSlot(UBEPlayerLoadoutSave::StaticClass(), FString());
	}

	return false;
}
