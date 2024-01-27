// Copyright (C) 2024 owoDra

#include "BEGameState.h"

#include "ProjectBELogs.h"

// Game Loading Core
#include "LoadingScreenSubsystem.h"
#include "GameplayTag/GCLoadingTags_LoadingType.h"

// Game Experience Extension
#include "ExperienceDataComponent.h"

// Game Ability Extension
#include "GAEAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEGameState)


const FName ABEGameState::NAME_GameStateLoading("LoadingGameState");

ABEGameState::ABEGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ExperienceDataComponent = ObjectInitializer.CreateDefaultSubobject<UExperienceDataComponent>(this, TEXT("ExperienceData"));
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UGAEAbilitySystemComponent>(this, TEXT("AbilitySystem"));

	ExperienceDataComponent->OnGameReady_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::HandleGameReady));

	GameStateLoadingReason = FText(NSLOCTEXT("LoadingScreen", "GameStateLoadingReason", "Loading Game State"));
}


void ABEGameState::BeginPlay()
{
	if (auto* Subsystem{ UGameInstance::GetSubsystem<ULoadingScreenSubsystem>(GetGameInstance()) })
	{
		Subsystem->AddLoadingProcess(ABEGameState::NAME_GameStateLoading, TAG_LoadingType_Fullscreen, GameStateLoadingReason);
	}

	Super::BeginPlay();
}

void ABEGameState::HandleGameReady()
{
	if (auto* Subsystem{ UGameInstance::GetSubsystem<ULoadingScreenSubsystem>(GetGameInstance()) })
	{
		Subsystem->RemoveLoadingProcess(ABEGameState::NAME_GameStateLoading);
	}
}


UAbilitySystemComponent* ABEGameState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
