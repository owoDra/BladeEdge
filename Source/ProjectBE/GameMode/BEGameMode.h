﻿// Copyright (C) 2023 owoDra

#pragma once

#include "Actor/GFCGameMode.h"

#include "BEGameMode.generated.h"


/** 
 * Base GameMode class of this project.
 */
UCLASS(Blueprintable)
class PROJECTBE_API ABEGameMode : public AGFCGameModeBase
{
	GENERATED_BODY()
public:
	ABEGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};