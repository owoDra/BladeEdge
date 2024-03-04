// Copyright (C) 2024 owoDra

#pragma once

#include "GameMode/GFCGameMode.h"

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

public:
	virtual bool ShouldSpawnAtStartSpot(AController* Player) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
};
