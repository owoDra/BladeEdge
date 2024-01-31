// Copyright (C) 2024 owoDra

#include "BECharacterRecipe_Loadout.h"

#include "Loadout/BELoadoutComponent.h"
#include "Item/BEEquipmentItemData.h"
#include "GameplayTag/BETags_Equipment.h"

// Game Character Extension
#include "CharacterInitStateComponent.h"
#include "GCExtLogs.h"

// Game Equipment Extension
#include "EquipmentFunctionLibrary.h"
#include "EquipmentManagerComponent.h"
#include "EquipmentSet.h"

// Engine Feature
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BECharacterRecipe_Loadout)


UBECharacterRecipe_Loadout::UBECharacterRecipe_Loadout()
{
	InstancingPolicy = ECharacterRecipeInstancingPolicy::Instanced;
	NetExecutionPolicy = ECharacterRecipeNetExecutionPolicy::ServerOnly;

#if WITH_EDITOR
	StaticClass()->FindPropertyByName(FName{ TEXTVIEW("InstancingPolicy") })->SetPropertyFlags(CPF_DisableEditOnTemplate);
	StaticClass()->FindPropertyByName(FName{ TEXTVIEW("NetExecutionPolicy") })->SetPropertyFlags(CPF_DisableEditOnTemplate);
#endif
}


void UBECharacterRecipe_Loadout::StartSetup_Implementation(const FCharacterRecipePawnInfo& Info)
{
	ListenLoadoutChange();

	FinishSetup();
}

void UBECharacterRecipe_Loadout::OnDestroy_Implementation()
{
	UnlistenLoadoutChange();
}


void UBECharacterRecipe_Loadout::ListenLoadoutChange()
{
	auto* PlayerState{ GetTypedPawn<APawn>()->GetPlayerState() };
	auto* LoadoutComponent{ PlayerState ? PlayerState->FindComponentByClass<UBELoadoutComponent>() : nullptr};

	if (LoadoutComponent)
	{
		LoadoutComponent->OnLoadoutChange.AddUObject(this, &ThisClass::HandleLoadoutChange);
		HandleLoadoutChange(LoadoutComponent);
	}
	else
	{
		HandleApplyDefault();
	}
}

void UBECharacterRecipe_Loadout::UnlistenLoadoutChange()
{
	auto* Pawn{ GetTypedPawn<APawn>() };
	auto* PlayerState{ Pawn ? Pawn->GetPlayerState() : nullptr };
	auto* LoadoutComponent{ PlayerState ? PlayerState->FindComponentByClass<UBELoadoutComponent>() : nullptr };

	if (LoadoutComponent)
	{
		LoadoutComponent->OnLoadoutChange.RemoveAll(this);
	}
}

void UBECharacterRecipe_Loadout::HandleLoadoutChange(const UBELoadoutComponent* LoadoutComponent)
{
	check(LoadoutComponent);

	if (auto* EMC{ UEquipmentFunctionLibrary::GetEquipmentManagerComponentFromActor(GetTypedPawn<APawn>()) })
	{
		// 先にすでに装備しているアイテムをすべて削除する

		UE_LOG(LogGameExt_CharacterRecipe, Log, TEXT("-- Remove All Equipment currently added"));

		EMC->RemoveAllEquipmentItem();

		// 新しくロードアウトのアイテムを装備する

		const auto& Loadout{ LoadoutComponent->Loadout };

		auto AddToManager
		{
			[&EMC](const UBEEquipmentItemData* Item)
			{
				if (Item)
				{
					FActiveEquipmentHandle DummyHundle;
					EMC->AddEquipmentItem(Item->GetSlotTag(), Item, DummyHundle);

					UE_LOG(LogGameExt_CharacterRecipe, Log, TEXT("++ Add Loadout Item(%s)"), *GetNameSafe(Item));
				}
			}
		};

		AddToManager(Loadout.FighterData);
		AddToManager(Loadout.WeaponData);
		AddToManager(Loadout.MainSkillData);
		AddToManager(Loadout.SubSkillData);
		AddToManager(Loadout.UltimateSkillData);

		// アクティブスロットを設定

		EMC->EquipEquipmentBySlot(TAG_Equipment_Slot_Weapon);
	}
}

void UBECharacterRecipe_Loadout::HandleApplyDefault()
{
	if (auto* EMC{ UEquipmentFunctionLibrary::GetEquipmentManagerComponentFromActor(GetTypedPawn<APawn>()) })
	{
		auto* LoadedEquipmentSet
		{
			DefaultEquipmentSet.IsNull() ? nullptr :
			DefaultEquipmentSet.IsValid() ? DefaultEquipmentSet.Get() : DefaultEquipmentSet.LoadSynchronous()
		};

		if (LoadedEquipmentSet)
		{
			UE_LOG(LogGameExt_CharacterRecipe, Log, TEXT("++DefaultEquipmentSet (Name: %s)"), *GetNameSafe(LoadedEquipmentSet));

			TArray<FActiveEquipmentHandle> DummyHandles;
			LoadedEquipmentSet->AddEquipmentItems(EMC, DummyHandles);
		}
	}
}
