// Copyright (C) 2023 owoDra

#include "BEGameState.h"

// Game Experience Extension
#include "ExperienceDataComponent.h"

// Game Ability Extension
#include "GAEAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEGameState)


ABEGameState::ABEGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ExperienceDataComponent = ObjectInitializer.CreateDefaultSubobject<UExperienceDataComponent>(this, TEXT("ExperienceData"));
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UGAEAbilitySystemComponent>(this, TEXT("AbilitySystem"));
}


UAbilitySystemComponent* ABEGameState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
