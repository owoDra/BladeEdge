// Copyright (C) 2024 owoDra

#include "BEEquipmentFragment_WeaponSkin.h"

#include "GameplayTag/BETags_MeshType.h"

// Game Character Extension
#include "Character/CharacterMeshAccessorInterface.h"

// Engine Feature
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Pawn.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEEquipmentFragment_WeaponSkin)

//////////////////////////////////////////////////////////////////////////////////

bool FBEEquipmentMeshToSpawn::IsValid() const
{
	return ::IsValid(MeshToSpawn);
}

//////////////////////////////////////////////////////////////////////////////////

UBEEquipmentFragment_WeaponSkin::UBEEquipmentFragment_WeaponSkin(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NetExecutionPolicy = EEquipmentFragmentNetExecutionPolicy::Both;

#if WITH_EDITOR
	StaticClass()->FindPropertyByName(FName{ TEXTVIEW("NetExecutionPolicy") })->SetPropertyFlags(CPF_DisableEditOnTemplate);
#endif
}

#if WITH_EDITOR 
EDataValidationResult UBEEquipmentFragment_WeaponSkin::IsDataValid(FDataValidationContext& Context) const
{
	auto Result{ CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid) };

	int32 Index{ 0 };
	for (const auto& Entry : MeshesToSpawn)
	{
		if (!Entry.IsValid())
		{
			Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);

			Context.AddError(FText::FromString(FString::Printf(TEXT("Invalid info in MeshesToSpawn[%d] in %s"), Index, *GetNameSafe(this))));
		}

		++Index;
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
		if (auto* TPPMesh{ ICharacterMeshAccessorInterface::Execute_GetMeshByTag(Pawn, TAG_MeshType_TPP) })
		{
			SpawmMeshesFor(Pawn, TPPMesh, false);
			SetAnimOverlay_TPP(TPPMesh);
		}

		if (auto* FPPMesh{ ICharacterMeshAccessorInterface::Execute_GetMeshByTag(Pawn, TAG_MeshType_FPP) })
		{
			SpawmMeshesFor(Pawn, FPPMesh, true);
			SetAnimOverlay_FPP(FPPMesh);
		}
	}
}

void UBEEquipmentFragment_WeaponSkin::HandleUnequiped()
{
	Super::HandleUnequiped();

	RemoveSpawnedMeshes();
	RemoveAnimOverlay_TPP();
	RemoveAnimOverlay_FPP();
}


// Spawn meshes

void UBEEquipmentFragment_WeaponSkin::SpawmMeshesFor(APawn* Pawn, USkeletalMeshComponent* TargetMesh, bool bIsFPP)
{
	// 専用サーバーでは作成しない

	if (Pawn->GetNetMode() == NM_DedicatedServer)
	{
		return;
	}

	const auto bOwnerNoSee{ static_cast<bool>(TargetMesh->bOwnerNoSee) };
	const auto bOnlyOwnerSee{ static_cast<bool>(TargetMesh->bOnlyOwnerSee) };
	const auto bHiddenInGame{ static_cast<bool>(TargetMesh->bHiddenInGame) };
	const auto bCastShadow{ static_cast<bool>(TargetMesh->CastShadow) };

	for (const auto& Entry : MeshesToSpawn)
	{
		auto* NewMesh{ NewObject<USkeletalMeshComponent>(Pawn) };
		NewMesh->SetSkeletalMesh(Entry.MeshToSpawn);
		NewMesh->SetAnimInstanceClass(Entry.MeshAnimInstance);
		NewMesh->AttachToComponent(TargetMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Entry.AttachSocket);
		NewMesh->SetRelativeTransform(Entry.AttachTransform);
		NewMesh->SetScalarParameterValueOnMaterials(FName(TEXTVIEW("IsFPP")), bIsFPP ? 1.0f : 0.0f);
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
	if (TPPAnimOverlay)
	{
		AppliedTPPMesh = TargetMesh;

		TargetMesh->LinkAnimClassLayers(TPPAnimOverlay);
		
		ListenMeshAnimInitialized_TPP(TargetMesh);
	}
}

void UBEEquipmentFragment_WeaponSkin::RemoveAnimOverlay_TPP()
{
	if (TPPAnimOverlay && AppliedTPPMesh.IsValid())
	{
		UnlistenMeshAnimInitialized_TPP(AppliedTPPMesh.Get());

		AppliedTPPMesh->UnlinkAnimClassLayers(TPPAnimOverlay);

		AppliedTPPMesh.Reset();
	}
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
	if (AppliedTPPMesh.IsValid())
	{
		AppliedTPPMesh->LinkAnimClassLayers(TPPAnimOverlay);
	}
}


// Apply Anim Overlay FPP

void UBEEquipmentFragment_WeaponSkin::SetAnimOverlay_FPP(USkeletalMeshComponent* TargetMesh)
{
	if (FPPAnimOverlay)
	{
		AppliedFPPMesh = TargetMesh;

		TargetMesh->LinkAnimClassLayers(FPPAnimOverlay);

		ListenMeshAnimInitialized_FPP(TargetMesh);
	}
}

void UBEEquipmentFragment_WeaponSkin::RemoveAnimOverlay_FPP()
{
	if (FPPAnimOverlay && AppliedFPPMesh.IsValid())
	{
		UnlistenMeshAnimInitialized_FPP(AppliedFPPMesh.Get());

		AppliedFPPMesh->UnlinkAnimClassLayers(FPPAnimOverlay);

		AppliedFPPMesh.Reset();
	}
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
	if (AppliedFPPMesh.IsValid())
	{
		AppliedFPPMesh->LinkAnimClassLayers(FPPAnimOverlay);
	}
}
