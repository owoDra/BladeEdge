// Copyright (C) 2024 owoDra

#include "BECharacterRecipe_BasicSetting.h"

// Game Character Extension
#include "CharacterInitStateComponent.h"
#include "GCExtLogs.h"

// Game Ability Extension
#include "GAEAbilitySystemComponent.h"
#include "AbilityTagRelationshipMapping.h"

// Game Ability: Health Addon
#include "HealthFunctionLibrary.h"
#include "HealthComponent.h"
#include "HealthData.h"

// Game Locomotion Extension
#include "LocomotionComponent.h"
#include "LocomotionData.h"

// Game View Extension
#include "ViewerComponent.h"
#include "Mode/ViewMode.h"

// Engine Features
#include "AbilitySystemGlobals.h"
#include "GameFramework/Character.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BECharacterRecipe_BasicSetting)


UBECharacterRecipe_BasicSetting::UBECharacterRecipe_BasicSetting()
{
	InstancingPolicy = ECharacterRecipeInstancingPolicy::NonInstanced;
	NetExecutionPolicy = ECharacterRecipeNetExecutionPolicy::Both;

#if WITH_EDITOR
	StaticClass()->FindPropertyByName(FName{ TEXTVIEW("InstancingPolicy") })->SetPropertyFlags(CPF_DisableEditOnTemplate);
	StaticClass()->FindPropertyByName(FName{ TEXTVIEW("NetExecutionPolicy") })->SetPropertyFlags(CPF_DisableEditOnTemplate);
#endif
}


void UBECharacterRecipe_BasicSetting::StartSetupNonInstanced_Implementation(FCharacterRecipePawnInfo Info) const
{
	auto* Pawn{ Info.Pawn.Get() };

	// Server Only

	if (Pawn->HasAuthority())
	{
		ServerOnlyProcess(Pawn);
	}

	// Client Only

	if (Pawn->GetNetMode() != NM_DedicatedServer)
	{
		ClientOnlyProcess(Pawn);
	}

	// Both

	BothProcess(Pawn);
}

void UBECharacterRecipe_BasicSetting::ServerOnlyProcess(APawn* Pawn) const
{
	AbilityProcess(Pawn);
	HealthProcess(Pawn);
}

void UBECharacterRecipe_BasicSetting::ClientOnlyProcess(APawn* Pawn) const
{
	ViewProcess(Pawn);
}

void UBECharacterRecipe_BasicSetting::BothProcess(APawn* Pawn) const
{
	LocomotionProcess(Pawn);
}


void UBECharacterRecipe_BasicSetting::AbilityProcess(APawn* Pawn) const
{
	if (auto* ASC{ UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn) })
	{
		for (const auto& AbilitySetSoftObject : BasicAbilities.AbilitySets)
		{
			auto* AbilitySet
			{
				AbilitySetSoftObject.IsNull() ? nullptr :
				AbilitySetSoftObject.IsValid() ? AbilitySetSoftObject.Get() : AbilitySetSoftObject.LoadSynchronous()
			};

			UE_LOG(LogGameExt_CharacterRecipe, Log, TEXT("++AbilitySet (Name: %s)"), *GetNameSafe(AbilitySet));

			AbilitySet->GiveToAbilitySystem(ASC, nullptr);
		}

		if (auto* GAEASC{ Cast<UGAEAbilitySystemComponent>(ASC) })
		{
			if (!BasicAbilities.TagRelationshipMapping.IsNull())
			{
				const auto* LoadedTagRelationshipMapping
				{
					BasicAbilities.TagRelationshipMapping.IsValid() ? BasicAbilities.TagRelationshipMapping.Get() : BasicAbilities.TagRelationshipMapping.LoadSynchronous()
				};

				UE_LOG(LogGameExt_CharacterRecipe, Log, TEXT("++TagRelationshipMapping (Name: %s)"), *GetNameSafe(LoadedTagRelationshipMapping));

				GAEASC->SetTagRelationshipMapping(LoadedTagRelationshipMapping);
			}
		}
	}
}

void UBECharacterRecipe_BasicSetting::HealthProcess(APawn* Pawn) const
{
	if (auto* HC{ UHealthFunctionLibrary::GetHealthComponentFromActor(Pawn) })
	{
		auto* LoadedHealthData
		{
			BasicHealth.HealthData.IsNull() ? nullptr :
			BasicHealth.HealthData.IsValid() ? BasicHealth.HealthData.Get() : BasicHealth.HealthData.LoadSynchronous()
		};

		UE_LOG(LogGameExt_CharacterRecipe, Log, TEXT("++HealthData (Name: %s)"), *GetNameSafe(LoadedHealthData));

		HC->SetHealthData(LoadedHealthData);
	}
}

void UBECharacterRecipe_BasicSetting::LocomotionProcess(APawn* Pawn) const
{
	if (auto* Character{ Cast<ACharacter>(Pawn) })
	{
		if (auto* LC{ ULocomotionComponent::FindLocomotionComponent(Character) })
		{
			auto* LoadedLocomotionData
			{
				BasicLocomotion.LocomotionData.IsNull() ? nullptr :
				BasicLocomotion.LocomotionData.IsValid() ? BasicLocomotion.LocomotionData.Get() : BasicLocomotion.LocomotionData.LoadSynchronous()
			};

			UE_LOG(LogGameExt_CharacterRecipe, Log, TEXT("++LocomotionData (Name: %s)"), *GetNameSafe(LoadedLocomotionData));

			LC->SetLocomotionData(LoadedLocomotionData);
		}
	}
}

void UBECharacterRecipe_BasicSetting::ViewProcess(APawn* Pawn) const
{
	if (auto* VC{ Pawn->FindComponentByClass<UViewerComponent>() })
	{
		auto* LoadedViewModeClass
		{
			BasicView.ViewMode.IsNull() ? nullptr :
			BasicView.ViewMode.IsValid() ? BasicView.ViewMode.Get() : BasicView.ViewMode.LoadSynchronous()
		};

		UE_LOG(LogGameExt_CharacterRecipe, Log, TEXT("++ViewMode (Name: %s)"), *GetNameSafe(LoadedViewModeClass));

		auto ViewModeClass{ TSubclassOf<UViewMode>(LoadedViewModeClass) };

		VC->InitializeViewMode(ViewModeClass);
	}
}
