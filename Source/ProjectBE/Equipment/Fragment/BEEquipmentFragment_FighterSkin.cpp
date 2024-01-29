// Copyright (C) 2024 owoDra

#include "BEEquipmentFragment_FighterSkin.h"

#include "Loadout/BELoadoutComponent.h"
#include "GameplayTag/BETags_MeshType.h"
#include "GameplayTag/BETags_Equipment.h"

// Game Character Extension
#include "Character/CharacterMeshAccessorInterface.h"

// Game Effect Extension
#include "ContextEffectComponent.h"
#include "ContextEffectLibrary.h"

// Engine Feature
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/Pawn.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEEquipmentFragment_FighterSkin)


UBEEquipmentFragment_FighterSkin::UBEEquipmentFragment_FighterSkin(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NetExecutionPolicy = EEquipmentFragmentNetExecutionPolicy::ClientOnly;

#if WITH_EDITOR
	StaticClass()->FindPropertyByName(FName{ TEXTVIEW("NetExecutionPolicy") })->SetPropertyFlags(CPF_DisableEditOnTemplate);
#endif
}

#if WITH_EDITOR 
EDataValidationResult UBEEquipmentFragment_FighterSkin::IsDataValid(FDataValidationContext& Context) const
{
auto Result{ CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid) };

	if (DataTable)
	{
		if (DataTable->GetRowNames().IsEmpty())
		{
			Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);

			Context.AddError(FText::FromString(FString::Printf(TEXT("Emptry Data table in %s"), *GetNameSafe(this))));
		}
		else
		{
			DataTable->ForeachRow<FBEDataRow_FighterSkin>(FString(),
				[this, &Result, &Context](const FName& Name, const FBEDataRow_FighterSkin& Row)
				{
					if (!Row.IsValid())
					{
						Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);

						Context.AddError(
							FText::FromString(FString::Printf(TEXT("Invalid row(%s) in table(%s) in %s")
								, *Name.ToString()
								, *GetNameSafe(DataTable)
								, *GetNameSafe(this))));
					}
				}
			);
		}
	}
	else
	{
		Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);

		Context.AddError(FText::FromString(FString::Printf(TEXT("Invalid Data table in %s"), *GetNameSafe(this))));
	}

	return Result;
}
#endif


// Equiped / Unequiped

void UBEEquipmentFragment_FighterSkin::HandleEquiped()
{
	Super::HandleEquiped();

	if (auto* Pawn{ GetOwner<APawn>() })
	{
		StoreSkinData(Pawn);

		if (auto* TPPMesh{ ICharacterMeshAccessorInterface::Execute_GetMeshByTag(Pawn, TAG_MeshType_TPP) })
		{
			SetMesh(TPPMesh, SkinData.TPPMeshesToSet);
		}

		if (auto* FPPMesh{ ICharacterMeshAccessorInterface::Execute_GetMeshByTag(Pawn, TAG_MeshType_FPP) })
		{
			SetMesh(FPPMesh, SkinData.FPPMeshesToSet);
		}

		SetContextEffectLibraries(Pawn);
	}
}


// Skin Data

void UBEEquipmentFragment_FighterSkin::StoreSkinData(APawn* Pawn)
{
	auto* PlayerState{ Pawn->GetPlayerState() };
	ensure(PlayerState);

	if (auto* LoadoutComponent{ PlayerState ? PlayerState->GetComponentByClass<UBELoadoutComponent>() : nullptr })
	{
		auto SkinName{ LoadoutComponent->GetSkinNameBySlot(TAG_Equipment_Slot_Fighter) };

		// 取得したスキン名が None じゃなければそれでデータを取得

		if (!SkinName.IsNone())
		{
			if (auto * FoundRow{ DataTable->FindRow<FBEDataRow_FighterSkin>(SkinName, FString()) })
			{
				SkinData = *FoundRow;
				return;
			}
		}

		// 取得したスキン名では見つからなかった場合はデフォルトを返す

		SkinName = DataTable->GetRowNames()[0];
		auto DefaultRow{ DataTable->FindRow<FBEDataRow_FighterSkin>(SkinName, FString()) };
		check(DefaultRow);

		SkinData = *DefaultRow;
	}
}


// Set Meshes

void UBEEquipmentFragment_FighterSkin::SetMesh(USkeletalMeshComponent* TargetMesh, const FBEMeshToSetMesh& Info)
{
	auto* LoadedSkeltalMesh
	{
		Info.SkeletalMesh.IsNull() ? nullptr :
		Info.SkeletalMesh.IsValid() ? Info.SkeletalMesh.Get() : Info.SkeletalMesh.LoadSynchronous()
	};

	TargetMesh->SetSkeletalMesh(LoadedSkeltalMesh);

	auto* LoadedAnimInstanceClass
	{
		Info.AnimInstance.IsNull() ? nullptr :
		Info.AnimInstance.IsValid() ? Info.AnimInstance.Get() : Info.AnimInstance.LoadSynchronous()
	};

	TargetMesh->SetAnimInstanceClass(LoadedAnimInstanceClass);

	TargetMesh->SetRelativeLocation(Info.NewLocation);
	TargetMesh->SetRelativeRotation(Info.NewRotation);
	TargetMesh->SetRelativeScale3D(Info.NewScale);
}


// Set Context Effect

void UBEEquipmentFragment_FighterSkin::SetContextEffectLibraries(APawn* Pawn)
{
	if (auto* Component{ Pawn->FindComponentByClass<UContextEffectComponent>() })
	{
		Component->UpdateContextEffectLibraries(SkinData.ContextEffectLibraries);
		Component->UpdateExtraContexts(SkinData.ExtraContexts);
	}
}
