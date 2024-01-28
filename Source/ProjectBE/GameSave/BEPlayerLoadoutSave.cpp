// Copyright (C) 2024 owoDra

#include "BEPlayerLoadoutSave.h"

#include "DeveloperSettings/BELoadoutDeveloperSettings.h"
#include "GameplayTag/BETags_LoadingType.h"

// Game Loading Core
#include "LoadingScreenSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEPlayerLoadoutSave)


const FName UBEPlayerLoadoutSave::NAME_LoadoutLoading("LoadoutLoading");

void UBEPlayerLoadoutSave::ResetToDefault()
{
	Super::ResetToDefault();

	FighterSkin.Empty();
	WeaponSkin.Empty();

	const auto* DevSettings{ GetDefault<UBELoadoutDeveloperSettings>() };

	Fighter			= DevSettings->DefaultFighter;
	Weapon			= DevSettings->DefaultWeapon;
	MainSkill		= DevSettings->DefaultMainSkill;
	SubSkill		= DevSettings->DefaultSubSkill;
	UltimateSkill	= DevSettings->DefaultUltimateSkill;
}

void UBEPlayerLoadoutSave::HandlePreSave()
{
	if (auto* Subsystem{ UGameInstance::GetSubsystem<ULoadingScreenSubsystem>(GetLocalPlayer()->GetGameInstance()) })
	{
		Subsystem->AddLoadingProcess(
			UBEPlayerLoadoutSave::NAME_LoadoutLoading
			, TAG_LoadingType_AsyncIndicator
			, NSLOCTEXT("LoadingScreen", "LoadoutLoadingReason", "Saving Loadout"));
	}
}

void UBEPlayerLoadoutSave::HandlePostSave(bool bSuccess)
{
	if (auto* Subsystem{ UGameInstance::GetSubsystem<ULoadingScreenSubsystem>(GetLocalPlayer()->GetGameInstance()) })
	{
		Subsystem->RemoveLoadingProcess(UBEPlayerLoadoutSave::NAME_LoadoutLoading);
	}
}
