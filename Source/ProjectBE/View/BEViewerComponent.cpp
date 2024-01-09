// Copyright (C) 2024 owoDra

#include "BEViewerComponent.h"

#include "GameSetting/GameplaySettingSubsystem.h"

#include "GSCGameUserSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEViewerComponent)


UBEViewerComponent::UBEViewerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


void UBEViewerComponent::BeginPlay()
{
	InitializeFromGameSettings();

	Super::BeginPlay();
}

void UBEViewerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UninitializeFromGameSettings();

	Super::EndPlay(EndPlayReason);
}


void UBEViewerComponent::InitializeFromGameSettings()
{
	if (auto* Settings{ UGSCGameUserSettings::GetGSCGameUserSettings() })
	{
		Settings->CallAndRegister_OnGameSettingsApplied(
			UGSCGameUserSettings::FGameSettingsAppliedDelegate::FDelegate::CreateUObject(this, &ThisClass::HandleSettingApplied));
	}
}

void UBEViewerComponent::UninitializeFromGameSettings()
{
	if (auto* Settings{ UGSCGameUserSettings::GetGSCGameUserSettings() })
	{
		Settings->OnGameSettingsApplied.RemoveAll(this);
	}
}

void UBEViewerComponent::HandleSettingApplied(UGSCGameUserSettings* Settings)
{
	check(Settings);

	auto* GameplaySettings{ Settings->GetSubsystem<UGameplaySettingSubsystem>() };
	check(GameplaySettings);

	CurrentFieldOfView = GameplaySettings->GetFieldOfView();
}


void UBEViewerComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	Super::GetCameraView(DeltaTime, DesiredView);

	DesiredView.FOV = CurrentFieldOfView;
}
