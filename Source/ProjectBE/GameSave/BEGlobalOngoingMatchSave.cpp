// Copyright (C) 2024 owoDra

#include "BEGlobalOngoingMatchSave.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEGlobalOngoingMatchSave)


void UBEGlobalOngoingMatchSave::ResetToDefault()
{
	Super::ResetToDefault();

	OngoingLobbyUserfacingExperience = FPrimaryAssetId();
	OngoingLobbyUniqueId.Empty();
}
