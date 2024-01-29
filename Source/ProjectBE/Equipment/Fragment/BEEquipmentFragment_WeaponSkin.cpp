// Copyright (C) 2024 owoDra

#include "BEEquipmentFragment_WeaponSkin.h"

#include "Loadout/BELoadoutComponent.h"
#include "GameplayTag/BETags_MeshType.h"
#include "GameplayTag/BETags_Equipment.h"

// Game Character Extension
#include "Character/CharacterMeshAccessorInterface.h"

// Engine Feature
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/Pawn.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEEquipmentFragment_WeaponSkin)


UBEEquipmentFragment_WeaponSkin::UBEEquipmentFragment_WeaponSkin(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NetExecutionPolicy = EEquipmentFragmentNetExecutionPolicy::ClientOnly;

#if WITH_EDITOR
	StaticClass()->FindPropertyByName(FName{ TEXTVIEW("NetExecutionPolicy") })->SetPropertyFlags(CPF_DisableEditOnTemplate);
#endif
}

#if WITH_EDITOR 
EDataValidationResult UBEEquipmentFragment_WeaponSkin::IsDataValid(FDataValidationContext& Context) const
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
			DataTable->ForeachRow<FBEDataRow_WeaponSkin>(FString(),
				[this, &Result, &Context](const FName& Name, const FBEDataRow_WeaponSkin& Row)
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

void UBEEquipmentFragment_WeaponSkin::HandleEquiped()
{
	Super::HandleEquiped();

	if (auto* Pawn{ GetOwner<APawn>() })
	{
		StoreSkinData(Pawn);

		if (auto* TPPMesh{ ICharacterMeshAccessorInterface::Execute_GetMeshByTag(Pawn, TAG_MeshType_TPP) })
		{
			SpawmMeshesFor(Pawn, TPPMesh);
			SetAnimOverlay_TPP(TPPMesh);
			ListenMeshAnimInitialized_TPP(TPPMesh);
		}

		if (auto* FPPMesh{ ICharacterMeshAccessorInterface::Execute_GetMeshByTag(Pawn, TAG_MeshType_FPP) })
		{
			SpawmMeshesFor(Pawn, FPPMesh);
			SetAnimOverlay_FPP(FPPMesh);
			ListenMeshAnimInitialized_FPP(FPPMesh);
		}
	}
}

void UBEEquipmentFragment_WeaponSkin::HandleUnequiped()
{
	Super::HandleUnequiped();

	RemoveSpawnedMeshes();
	RemoveAnimOverlay_TPP();
	RemoveAnimOverlay_FPP();

	if (auto* Pawn{ GetOwner<APawn>() })
	{
		if(auto* TPPMesh{ ICharacterMeshAccessorInterface::Execute_GetMeshByTag(Pawn, TAG_MeshType_TPP) })
		{
			UnlistenMeshAnimInitialized_TPP(TPPMesh);
		}
		
		if (auto* FPPMesh{ ICharacterMeshAccessorInterface::Execute_GetMeshByTag(Pawn, TAG_MeshType_FPP) })
		{
			UnlistenMeshAnimInitialized_FPP(FPPMesh);
		}
	}
}


// Skin Data

void UBEEquipmentFragment_WeaponSkin::StoreSkinData(APawn* Pawn)
{
	auto* PlayerState{ Pawn->GetPlayerState() };
	ensure(PlayerState);

	if (auto* LoadoutComponent{ PlayerState ? PlayerState->GetComponentByClass<UBELoadoutComponent>() : nullptr})
	{
		auto SkinName{ LoadoutComponent->GetSkinNameBySlot(TAG_Equipment_Slot_Weapon) };
		
		// 取得したスキン名が None じゃなければそれでデータを取得

		if (!SkinName.IsNone())
		{
			if (auto* FoundRow{ DataTable->FindRow<FBEDataRow_WeaponSkin>(SkinName, FString()) })
			{
				SkinData = *FoundRow;
				return;
			}
		}

		// 取得したスキン名では見つからなかった場合はデフォルトを返す

		SkinName = DataTable->GetRowNames()[0];
		auto DefaultRow{ DataTable->FindRow<FBEDataRow_WeaponSkin>(SkinName, FString()) };
		check(DefaultRow);

		SkinData = *DefaultRow;
	}
}


// Spawn meshes

void UBEEquipmentFragment_WeaponSkin::SpawmMeshesFor(APawn* Pawn, USkeletalMeshComponent* TargetMesh)
{
	for (const auto& Entry : SkinData.MeshesToSpawn)
	{
		const auto bOwnerNoSee{ static_cast<bool>(TargetMesh->bOwnerNoSee) };
		const auto bOnlyOwnerSee{ static_cast<bool>(TargetMesh->bOnlyOwnerSee) };
		const auto bHiddenInGame{ static_cast<bool>(TargetMesh->bHiddenInGame) };
		const auto bCastShadow{ static_cast<bool>(TargetMesh->CastShadow) };

		auto* MeshToSet
		{
			Entry.MeshToSpawn.IsValid() ? Entry.MeshToSpawn.Get() : Entry.MeshToSpawn.LoadSynchronous()
		};

		auto* AnimInstanceClass
		{
			Entry.MeshAnimInstance.IsNull() ? nullptr :
			Entry.MeshAnimInstance.IsValid() ? Entry.MeshAnimInstance.Get() : Entry.MeshAnimInstance.LoadSynchronous()
		};

		auto* NewMesh{ NewObject<USkeletalMeshComponent>(Pawn) };
		NewMesh->SetSkeletalMesh(MeshToSet);
		NewMesh->SetAnimInstanceClass(AnimInstanceClass);
		NewMesh->SetRelativeTransform(Entry.AttachTransform);
		NewMesh->AttachToComponent(TargetMesh, FAttachmentTransformRules::KeepRelativeTransform, Entry.AttachSocket);
		NewMesh->SetOwnerNoSee(bOwnerNoSee);
		NewMesh->SetOnlyOwnerSee(bOnlyOwnerSee);
		NewMesh->SetHiddenInGame(bHiddenInGame);
		NewMesh->SetCastShadow(bCastShadow);
		NewMesh->RegisterComponent();

		SpawnedMeshes.Add(NewMesh);
	}
}

void UBEEquipmentFragment_WeaponSkin::RemoveSpawnedMeshes()
{
	for (const auto& Mesh : SpawnedMeshes)
	{
		if (Mesh)
		{
			Mesh->DestroyComponent();
		}
	}

	SpawnedMeshes.Empty();
}


// Apply Anim Overlay

void UBEEquipmentFragment_WeaponSkin::SetAnimOverlay_TPP(USkeletalMeshComponent* TargetMesh)
{
	auto* OverlayClass
	{
		SkinData.TPPAnimOverlay.IsNull() ? nullptr :
		SkinData.TPPAnimOverlay.IsValid() ? SkinData.TPPAnimOverlay.Get() : SkinData.TPPAnimOverlay.LoadSynchronous()
	};

	if (OverlayClass)
	{
		TargetMesh->LinkAnimClassLayers(OverlayClass);

		ApplingOverlay_TPP.ApplingOverlayClass = OverlayClass;
		ApplingOverlay_TPP.TargetMesh = TargetMesh;
	}
}

void UBEEquipmentFragment_WeaponSkin::RemoveAnimOverlay_TPP()
{
	if (ApplingOverlay_TPP.TargetMesh.IsValid())
	{
		ApplingOverlay_TPP.TargetMesh->UnlinkAnimClassLayers(ApplingOverlay_TPP.ApplingOverlayClass);
	}

	ApplingOverlay_TPP.TargetMesh.Reset();
	ApplingOverlay_TPP.ApplingOverlayClass = nullptr;
}

void UBEEquipmentFragment_WeaponSkin::ListenMeshAnimInitialized_TPP(USkeletalMeshComponent* TargetMesh)
{
	TargetMesh->OnAnimInitialized.AddDynamic(this, &ThisClass::HandleMeshAnimInitialized_TPP);
}

void UBEEquipmentFragment_WeaponSkin::UnlistenMeshAnimInitialized_TPP(USkeletalMeshComponent* TargetMesh)
{
	TargetMesh->OnAnimInitialized.RemoveDynamic(this, &ThisClass::HandleMeshAnimInitialized_TPP);
}

void UBEEquipmentFragment_WeaponSkin::HandleMeshAnimInitialized_TPP()
{
	if (ApplingOverlay_TPP.TargetMesh.IsValid())
	{
		ApplingOverlay_TPP.TargetMesh->LinkAnimClassLayers(ApplingOverlay_TPP.ApplingOverlayClass);
	}
}


// Apply Anim Overlay FPP

void UBEEquipmentFragment_WeaponSkin::SetAnimOverlay_FPP(USkeletalMeshComponent* TargetMesh)
{
	auto* OverlayClass
	{
		SkinData.FPPAnimOverlay.IsNull() ? nullptr :
		SkinData.FPPAnimOverlay.IsValid() ? SkinData.FPPAnimOverlay.Get() : SkinData.FPPAnimOverlay.LoadSynchronous()
	};

	if (OverlayClass)
	{
		TargetMesh->LinkAnimClassLayers(OverlayClass);

		ApplingOverlay_FPP.ApplingOverlayClass = OverlayClass;
		ApplingOverlay_FPP.TargetMesh = TargetMesh;
	}
}

void UBEEquipmentFragment_WeaponSkin::RemoveAnimOverlay_FPP()
{
	if (ApplingOverlay_FPP.TargetMesh.IsValid())
	{
		ApplingOverlay_FPP.TargetMesh->UnlinkAnimClassLayers(ApplingOverlay_FPP.ApplingOverlayClass);
	}

	ApplingOverlay_FPP.TargetMesh.Reset();
	ApplingOverlay_FPP.ApplingOverlayClass = nullptr;
}

void UBEEquipmentFragment_WeaponSkin::ListenMeshAnimInitialized_FPP(USkeletalMeshComponent* TargetMesh)
{
	TargetMesh->OnAnimInitialized.AddDynamic(this, &ThisClass::HandleMeshAnimInitialized_FPP);
}

void UBEEquipmentFragment_WeaponSkin::UnlistenMeshAnimInitialized_FPP(USkeletalMeshComponent* TargetMesh)
{
	TargetMesh->OnAnimInitialized.RemoveDynamic(this, &ThisClass::HandleMeshAnimInitialized_FPP);
}

void UBEEquipmentFragment_WeaponSkin::HandleMeshAnimInitialized_FPP()
{
	if (ApplingOverlay_FPP.TargetMesh.IsValid())
	{
		ApplingOverlay_FPP.TargetMesh->LinkAnimClassLayers(ApplingOverlay_FPP.ApplingOverlayClass);
	}
}

