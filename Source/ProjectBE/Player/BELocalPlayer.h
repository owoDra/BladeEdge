// Copyright (C) 2024 owoDra

#pragma once

#include "Player/GFCLocalPlayer.h"

#include "BELocalPlayer.generated.h"


/**
 * Base LocalPlayer class of this project.
 */
UCLASS(Transient)
class PROJECTBE_API UBELocalPlayer : public UGFCLocalPlayer
{
	GENERATED_BODY()
public:
	UBELocalPlayer(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
