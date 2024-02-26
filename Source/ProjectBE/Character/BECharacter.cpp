// Copyright (C) 2024 owoDra

#include "BECharacter.h"

// Game Effect Extension
#include "ContextEffectComponent.h"

// Game Team Extesnion
#include "TeamFunctionLibrary.h"
#include "TeamMemberComponent.h"

// Game Character Extension
#include "CharacterInitStateComponent.h"

// Game Ability Extension
#include "GAEAbilitySystemComponent.h"

// Game Ability: Health Addon
#include "HealthComponent.h"

// Game Equipment Extension
#include "EquipmentManagerComponent.h"

// Engine Features
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"
#include "GameFramework/PlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BECharacter)


ABECharacter::ABECharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, StatTags(this)
{
	CharacterInitStateComponent = ObjectInitializer.CreateDefaultSubobject<UCharacterInitStateComponent>(this, TEXT("InitState"));
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UGAEAbilitySystemComponent>(this, TEXT("AbilitySystem"));
	HealthComponent = ObjectInitializer.CreateDefaultSubobject<UHealthComponent>(this, TEXT("Health"));
	EquipmentManagerComponent = ObjectInitializer.CreateDefaultSubobject<UEquipmentManagerComponent>(this, TEXT("EquipmentManager"));
	ContextEffectComponent = ObjectInitializer.CreateDefaultSubobject<UContextEffectComponent>(this, TEXT("ContextEffect"));

	FScriptDelegate NewDeathStartedDelegate;
	NewDeathStartedDelegate.BindUFunction(this, GET_FUNCTION_NAME_STRING_CHECKED(ABECharacter, HandleDeathStart));
	HealthComponent->OnDeathStarted.Add(NewDeathStartedDelegate);

	FScriptDelegate NewDeathFinishedDelegate;
	NewDeathFinishedDelegate.BindUFunction(this, GET_FUNCTION_NAME_STRING_CHECKED(ABECharacter, HandleDeathFinish));
	HealthComponent->OnDeathFinished.Add(NewDeathFinishedDelegate);

	CharacterInitStateComponent->OnGameReady_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::HandleGameReady));
}

void ABECharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	Params.Condition = COND_None;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, StatTags, Params);
}


void ABECharacter::HandleGameReady()
{
	OnGameReady();
}


#pragma region Death

void ABECharacter::HandleDeathStart()
{
	DisableMovementAndCollision();

	OnDeathStarted();
}

void ABECharacter::HandleDeathFinish()
{
	UninitAndDestroy();

	OnDeathFinished();
}

#pragma region


#pragma region Interface

UAbilitySystemComponent* ABECharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UTeamMemberComponent* ABECharacter::GetTeamMemberComponent() const
{
	return UTeamFunctionLibrary::GetTeamMemberComponentFromActor(GetPlayerState());
}

UEquipmentManagerComponent* ABECharacter::GetEquipmentManagerComponent() const
{
	return EquipmentManagerComponent;
}

UHealthComponent* ABECharacter::GetHealthComponent() const
{
	return HealthComponent;
}

void ABECharacter::PlayEffects_Implementation(
	FGameplayTag EffectTag
	, FGameplayTagContainer Contexts
	, USceneComponent* AttachToComponent
	, FName AttachPointName
	, FVector LocationOffset
	, FRotator RotationOffset
	, EAttachLocation::Type LocationType
	, float VolumeMultiplier
	, float PitchMultiplier
	, FVector VFXScale)
{
	IContextEffectInterface::Execute_PlayEffects(
		ContextEffectComponent
		, EffectTag
		, Contexts
		, AttachToComponent
		, AttachPointName
		, LocationOffset
		, RotationOffset
		, LocationType
		, VolumeMultiplier
		, PitchMultiplier
		, VFXScale
	);
}

#pragma endregion
