// Copyright (C) 2024 owoDra

#include "BEDataRow_FighterSkin.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEDataRow_FighterSkin)


bool FBEMeshToSetMesh::IsValid() const
{
    return !SkeletalMesh.IsNull() && !AnimInstance.IsNull();
}


bool FBEDataRow_FighterSkin::IsValid() const
{
    return FPPMeshesToSet.IsValid() && TPPMeshesToSet.IsValid();
}
