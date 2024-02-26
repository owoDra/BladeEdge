// Copyright (C) 2024 owoDra

#include "BEGameState.h"

#include "ProjectBELogs.h"

// Game Loading Core
#include "LoadingScreenSubsystem.h"
#include "GameplayTag/GCLoadingTags_LoadingType.h"

// Game Experience Extension
#include "ExperienceDataComponent.h"

// Game Phase Extension
#include "GamePhaseComponent.h"

// Game Ability Extension
#include "GAEAbilitySystemComponent.h"

// Engine Features
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEGameState)


const FName ABEGameState::NAME_GameStateLoading("LoadingGameState");

ABEGameState::ABEGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, StatTags(this)
{
	ExperienceDataComponent = ObjectInitializer.CreateDefaultSubobject<UExperienceDataComponent>(this, TEXT("ExperienceData"));
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UGAEAbilitySystemComponent>(this, TEXT("AbilitySystem"));
	GamePhaseComponent = ObjectInitializer.CreateDefaultSubobject<UGamePhaseComponent>(this, TEXT("GamePhase"));

	ExperienceDataComponent->OnGameReady_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::HandleGameReady));

	GameStateLoadingReason = FText(NSLOCTEXT("LoadingScreen", "GameStateLoadingReason", "Loading Game State"));
}

void ABEGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	Params.Condition = COND_None;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, StatTags, Params);
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
