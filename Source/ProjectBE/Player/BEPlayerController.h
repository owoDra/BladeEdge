﻿// Copyright (C) 2024 owoDra

#pragma once

#include "Player/GFCPlayerController.h"

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
