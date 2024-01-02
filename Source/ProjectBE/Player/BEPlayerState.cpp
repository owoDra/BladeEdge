// Copyright (C) 2023 owoDra

#include "BEPlayerState.h"

// Game Framework Core
#include "InitState/InitStateComponent.h"

// Game Ability Extension
#include "GAEAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEPlayerState)


ABEPlayerState::ABEPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InitStateComponent = ObjectInitializer.CreateDefaultSubobject<UInitStateComponent>(this, TEXT("InitState"));
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UGAEAbilitySystemComponent>(this, TEXT("AbilitySystem"));
}


UAbilitySystemComponent* ABEPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
