// Copyright (C) 2023 owoDra

#pragma once

#include "AbilityLocomotionCharacter.h"

#include "BECharacter.generated.h"


/** 
 * Base Character class of this project.
 */
UCLASS(Blueprintable)
class PROJECTBE_API ABECharacter : public AAbilityLocomotionCharacter
{
	GENERATED_BODY()
public:
	explicit ABECharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
