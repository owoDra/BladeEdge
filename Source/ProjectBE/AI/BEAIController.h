// Copyright (C) 2024 owoDra

#pragma once

#include "AI/GFCAIController.h"

#include "BEAIController.generated.h"


/** 
 * Base AIController class of this project.
 */
UCLASS(Blueprintable)
class PROJECTBE_API ABEAIController : public AGFCAIController
{
	GENERATED_BODY()
public:
	ABEAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
