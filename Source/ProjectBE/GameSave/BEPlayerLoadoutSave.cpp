// Copyright (C) 2024 owoDra

#include "BEPlayerLoadoutSave.h"

#include "DeveloperSettings/BELoadoutDeveloperSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEPlayerLoadoutSave)


void UBEPlayerLoadoutSave::ResetToDefault()
{
	Super::ResetToDefault();

	const auto* DevSettings{ GetDefault<UBELoadoutDeveloperSettings>() };

	Fighter			= DevSettings->Fighter;
	Weapon			= DevSettings->Weapon;
	MainSkill		= DevSettings->MainSkill;
	SubSkill		= DevSettings->SubSkill;
	UltimateSkill	= DevSettings->UltimateSkill;
}
