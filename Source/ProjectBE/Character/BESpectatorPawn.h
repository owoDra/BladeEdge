// Copyright (C) 2024 owoDra

#pragma once

#include "GameFramework/SpectatorPawn.h"

#include "BESpectatorPawn.generated.h"


/** 
 * Base SpectatorPawn class of this project.
 */
UCLASS(Blueprintable)
class PROJECTBE_API ABESpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()
public:
	ABESpectatorPawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
