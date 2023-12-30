// Copyright (C) 2023 owoDra

#pragma once

#include "System/GFCGameInstance.h"

#include "BEGameInstance.generated.h"


UCLASS(Blueprintable)
class PROJECTBE_API UBEGameInstance : public UGFCGameInstance
{
	GENERATED_BODY()
public:
	UBEGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
