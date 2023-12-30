// Copyright (C) 2023 owoDra

#pragma once

#include "Actor/GFCPlayerState.h"

#include "BEPlayerState.generated.h"


/** 
 * Base PlayerState class of this project.
 */
UCLASS(Blueprintable)
class PROJECTBE_API ABEPlayerState : public AGFCPlayerState
{
	GENERATED_BODY()
public:
	ABEPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
