// Copyright (C) 2024 owoDra

#include "BEPlayerState.h"

#include "Loadout/BELoadoutComponent.h"

// Game Framework Core
#include "InitState/InitStateComponent.h"

// Game Ability Extension
#include "GAEAbilitySystemComponent.h"

// Game Team Extension
#include "TeamMemberComponent.h"

// Engine Features
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEPlayerState)


ABEPlayerState::ABEPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, StatTags(this)
{
	InitStateComponent = ObjectInitializer.CreateDefaultSubobject<UInitStateComponent>(this, TEXT("InitState"));
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UGAEAbilitySystemComponent>(this, TEXT("AbilitySystem"));
	LoadoutComponent = ObjectInitializer.CreateDefaultSubobject<UBELoadoutComponent>(this, TEXT("Loadout"));
	TeamMemberComponent = ObjectInitializer.CreateDefaultSubobject<UTeamMemberComponent>(this, TEXT("TeamMember"));

	NetUpdateFrequency = 100.0f;
}

void ABEPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	Params.Condition = COND_None;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, StatTags, Params);
}


UAbilitySystemComponent* ABEPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UTeamMemberComponent* ABEPlayerState::GetTeamMemberComponent() const
{
	return TeamMemberComponent;
}
