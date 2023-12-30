// Copyright (C) 2023 owoDra

#pragma once

#include "Actor/GFCGameState.h"

#include "BEGameState.generated.h"


/** 
 * Base GameState class of this project.
 */
UCLASS(Blueprintable)
class PROJECTBE_API ABEGameState : public AGFCGameStateBase
{
	GENERATED_BODY()
public:
	ABEGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
