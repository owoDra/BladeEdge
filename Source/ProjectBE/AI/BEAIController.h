// Copyright (C) 2023 owoDra

#pragma once

#include "Actor/GFCAIController.h"

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
