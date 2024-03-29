﻿// Copyright (C) 2024 owoDra

#pragma once

#include "GameFramework/GameSession.h"

#include "BEGameSession.generated.h"


UCLASS()
class ABEGameSession : public AGameSession
{
	GENERATED_BODY()
public:
	ABEGameSession(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
