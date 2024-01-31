// Copyright (C) 2024 owoDra

#include "BEPlayerCharacter.h"

#include "View/BEViewerComponent.h"
#include "GameplayTag/BETags_MeshType.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEPlayerCharacter)


ABEPlayerCharacter::ABEPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ViewerComponent = ObjectInitializer.CreateDefaultSubobject<UBEViewerComponent>(this, TEXT("Camera"));
	ViewerComponent->SetupAttachment(GetRootComponent());

	FPPMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("FPPMesh"));
	FPPMesh->SetupAttachment(ViewerComponent);
	FPPMesh->SetOnlyOwnerSee(true);
	FPPMesh->SetCastShadow(false);
	FPPMesh->SetHiddenInGame(true);
}


void ABEPlayerCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	auto* PC{ Cast<APlayerController>(Controller) };
	auto PMC{ PC ? PC->PlayerCameraManager : nullptr };
	
	if (PMC)
	{
		FPPMesh->AttachToComponent(PMC->GetTransformComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}
	else
	{
		FPPMesh->AttachToComponent(ViewerComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}
}


TArray<USkeletalMeshComponent*> ABEPlayerCharacter::GetMeshes_Implementation() const
{
	return { GetMesh(), FPPMesh };
}

USkeletalMeshComponent* ABEPlayerCharacter::GetMainMesh_Implementation() const
{
	return GetMesh();
}

USkeletalMeshComponent* ABEPlayerCharacter::GetMeshByTag_Implementation(FGameplayTag Tag) const
{
	return (Tag == TAG_MeshType_TPP) ? GetMesh() : 
			(Tag == TAG_MeshType_FPP) ? FPPMesh.Get() : nullptr;
}
