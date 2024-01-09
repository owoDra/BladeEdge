// Copyright (C) 2024 owoDra

#include "BEGameState.h"

#include "ProjectBELogs.h"

// Game Experience Extension
#include "ExperienceDataComponent.h"

// Game UI Extension
#include "LoadingScreen/LoadingProcessTask.h"

// Game Ability Extension
#include "GAEAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEGameState)


const FString ABEGameState::GameStateLoadingReason("Loading Game Experience");

ABEGameState::ABEGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ExperienceDataComponent = ObjectInitializer.CreateDefaultSubobject<UExperienceDataComponent>(this, TEXT("ExperienceData"));
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UGAEAbilitySystemComponent>(this, TEXT("AbilitySystem"));

	ExperienceDataComponent->OnGameReady_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::HandleGameReady));
}


void ABEGameState::BeginPlay()
{
	LoadingProcessTask = ULoadingProcessTask::CreateLoadingScreenProcessTask(this, ABEGameState::GameStateLoadingReason);

	Super::BeginPlay();
}

void ABEGameState::HandleGameReady()
{
	if (LoadingProcessTask)
	{
		LoadingProcessTask->Unregister();
	}
}


UAbilitySystemComponent* ABEGameState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
