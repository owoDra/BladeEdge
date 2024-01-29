// Copyright (C) 2024 owoDra

#include "BEDataRow_WeaponSkin.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEDataRow_WeaponSkin)


///////////////////////////////////////////////////////////////
// FBEEquipmentMeshToSpawn

bool FBEEquipmentMeshToSpawn::IsValid() const
{
	return !MeshToSpawn.IsNull();
}


///////////////////////////////////////////////////////////////
// FBEDataRow_WeaponSkin

bool FBEDataRow_WeaponSkin::IsValid() const
{
	for (const auto& Entry : MeshesToSpawn)
	{
		if (!Entry.IsValid())
		{
			return false;
		}
	}

	return !FPPAnimOverlay.IsNull() && !TPPAnimOverlay.IsNull();
}
