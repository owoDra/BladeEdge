// Copyright (C) 2024 owoDra

#include "BEEquipmentFragment_FighterSkin.h"

#include "GameplayTag/BETags_MeshType.h"

// Game Character Extension
#include "Character/CharacterMeshAccessorInterface.h"

// Game Effect Extension
#include "ContextEffectComponent.h"
#include "ContextEffectLibrary.h"

// Engine Feature
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Pawn.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEEquipmentFragment_FighterSkin)

////////////////////////////////////////////////////////////////////////////

bool FBEMeshToSetMesh::IsValid() const
{
	return SkeletalMesh && AnimInstance;
}

////////////////////////////////////////////////////////////////////////////

UBEEquipmentFragment_FighterSkin::UBEEquipmentFragment_FighterSkin(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NetExecutionPolicy = EEquipmentFragmentNetExecutionPolicy::Both;

#if WITH_EDITOR
	StaticClass()->FindPropertyByName(FName{ TEXTVIEW("NetExecutionPolicy") })->SetPropertyFlags(CPF_DisableEditOnTemplate);
#endif
}

#if WITH_EDITOR 
EDataValidationResult UBEEquipmentFragment_FighterSkin::IsDataValid(FDataValidationContext& Context) const
{
	auto Result{ CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid) };

	if (!TPPMeshesToSet.IsValid())
	{
		Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);

		Context.AddError(FText::FromString(FString::Printf(TEXT("Invalid TPPMeshesToSet is set in %s"), *GetNameSafe(this))));
	}

	if (!FPPMeshesToSet.IsValid())
	{
		Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);

		Context.AddError(FText::FromString(FString::Printf(TEXT("Invalid FPPMeshesToSet is set in %s"), *GetNameSafe(this))));
	}

	return Result;
}
#endif


// Equiped / Unequiped

void UBEEquipmentFragment_FighterSkin::HandleEquipmentGiven()
{
	Super::HandleEquipmentGiven();

	if (auto* Pawn{ GetOwner<APawn>() })
	{
		if (auto* TPPMesh{ ICharacterMeshAccessorInterface::Execute_GetMeshByTag(Pawn, TAG_MeshType_TPP) })
		{
			SetMesh(TPPMesh, TPPMeshesToSet);
		}

		if (auto* FPPMesh{ ICharacterMeshAccessorInterface::Execute_GetMeshByTag(Pawn, TAG_MeshType_FPP) })
		{
			SetMesh(FPPMesh, FPPMeshesToSet);
		}

		SetContextEffectLibraries(Pawn);
	}
}



// Set Meshes

void UBEEquipmentFragment_FighterSkin::SetMesh(USkeletalMeshComponent* TargetMesh, const FBEMeshToSetMesh& Info)
{
	TargetMesh->SetSkeletalMesh(Info.SkeletalMesh);
	TargetMesh->SetAnimInstanceClass(Info.AnimInstance);
	TargetMesh->SetRelativeLocation(Info.NewLocation);
	TargetMesh->SetRelativeRotation(Info.NewRotation);
	TargetMesh->SetRelativeScale3D(Info.NewScale);
}


// Set Context Effect

void UBEEquipmentFragment_FighterSkin::SetContextEffectLibraries(APawn* Pawn)
{
	// 専用サーバーでは設定しない

	if (Pawn->GetNetMode() == NM_DedicatedServer)
	{
		return;
	}

	if (auto* Component{ Pawn->FindComponentByClass<UContextEffectComponent>() })
	{
		Component->UpdateContextEffectLibraries(ContextEffectLibraries);
		Component->UpdateExtraContexts(ExtraContexts);
	}
}
