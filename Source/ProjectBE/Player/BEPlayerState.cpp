// Copyright (C) 2024 owoDra

#include "BEPlayerState.h"

// Game Framework Core
#include "InitState/InitStateComponent.h"

// Game Ability Extension
#include "GAEAbilitySystemComponent.h"

// Engine Features
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEPlayerState)


ABEPlayerState::ABEPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, StatTags(this)
{
	InitStateComponent = ObjectInitializer.CreateDefaultSubobject<UInitStateComponent>(this, TEXT("InitState"));
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UGAEAbilitySystemComponent>(this, TEXT("AbilitySystem"));

	NetUpdateFrequency = 100.0f;
}

void ABEPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, StatTags);
}


UAbilitySystemComponent* ABEPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
