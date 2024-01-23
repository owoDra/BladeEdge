// Copyright (C) 2024 owoDra

#pragma once

#include "UI/GFCHUD.h"

#include "BEHUD.generated.h"


/**
 * Base HUD class of this project.
 */
UCLASS()
class PROJECTBE_API ABEHUD : public AGFCHUD
{
	GENERATED_BODY()
public:
	ABEHUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
