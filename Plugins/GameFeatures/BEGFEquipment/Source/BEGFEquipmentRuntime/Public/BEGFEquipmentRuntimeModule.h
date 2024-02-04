// Copyright (C) 2024 owoDra

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FBEGFEquipmentRuntimeModule : public IModuleInterface
{
public:
	//~IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	//~End of IModuleInterface
};
