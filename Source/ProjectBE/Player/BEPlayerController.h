// Copyright (C) 2023 owoDra

#pragma once

#include "Actor/GFCPlayerController.h"

#include "BEPlayerController.generated.h"


/** 
 * Base PlayerController class of this project.
 */
UCLASS(Blueprintable)
class PROJECTBE_API ABEPlayerController : public AGFCPlayerController
{
	GENERATED_BODY()
public:
	ABEPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
