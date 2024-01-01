// Copyright (C) 2023 owoDra

#pragma once

#include "ViewerComponent.h"

#include "BEViewerComponent.generated.h"

class UGSCGameUserSettings;


/**
 * Components that perform processing to enable the player to control the viewpoint
 */
UCLASS()
class PROJECTBE_API UBEViewerComponent : public UViewerComponent
{
	GENERATED_BODY()
public:
	UBEViewerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


protected:
	float CurrentFieldOfView{ 90.0f };

protected:
	virtual void InitializeFromGameSettings();
	virtual void UninitializeFromGameSettings();

	void HandleSettingApplied(UGSCGameUserSettings* Settings);


protected:
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

};
