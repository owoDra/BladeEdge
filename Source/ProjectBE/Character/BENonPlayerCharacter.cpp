// Copyright (C) 2024 owoDra

#include "BENonPlayerCharacter.h"

#include "GameplayTag/BETags_MeshType.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BENonPlayerCharacter)


ABENonPlayerCharacter::ABENonPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


TArray<USkeletalMeshComponent*> ABENonPlayerCharacter::GetMeshes_Implementation() const
{
	return { GetMesh() };
}

USkeletalMeshComponent* ABENonPlayerCharacter::GetMainMesh_Implementation() const
{
	return GetMesh();
}

USkeletalMeshComponent* ABENonPlayerCharacter::GetMeshByTag_Implementation(FGameplayTag Tag) const
{
	return (Tag == TAG_MeshType_TPP) ? GetMesh() : nullptr;
}
