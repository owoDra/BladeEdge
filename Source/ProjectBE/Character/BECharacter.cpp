// Copyright (C) 2023 owoDra

#include "BECharacter.h"

// Game Effect Extension
#include "ContextEffectComponent.h"

// Game Team Extesnion
#include "TeamFunctionLibrary.h"
#include "TeamMemberComponent.h"

// Game Character Extension
#include "CharacterDataComponent.h"

// Game Ability Extension
#include "GAEAbilitySystemComponent.h"

// Game UI Extension
#include "LoadingScreen/LoadingProcessTask.h"

// Game Ability: Health Addon
#include "HealthComponent.h"

// Game Ability: Equipment Addon
#include "EquipmentManagerComponent.h"

// Engine Features
#include "GameFramework/PlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BECharacter)


const FString ABECharacter::CharacterLoadingReason("Loading Character Data");

ABECharacter::ABECharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UGAEAbilitySystemComponent>(this, TEXT("AbilitySystem"));
	HealthComponent = ObjectInitializer.CreateDefaultSubobject<UHealthComponent>(this, TEXT("Health"));
	EquipmentManagerComponent = ObjectInitializer.CreateDefaultSubobject<UEquipmentManagerComponent>(this, TEXT("EquipmentManager"));
	ContextEffectComponent = ObjectInitializer.CreateDefaultSubobject<UContextEffectComponent>(this, TEXT("ContextEffect"));

	CharacterDataComponent->OnGameReady_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::HandleGameReady));
}


void ABECharacter::BeginPlay()
{
	LoadingProcessTask = ULoadingProcessTask::CreateLoadingScreenProcessTask(this, ABECharacter::CharacterLoadingReason);

	Super::BeginPlay();
}

void ABECharacter::HandleGameReady()
{
	if (LoadingProcessTask)
	{
		LoadingProcessTask->Unregister();
	}
}


UAbilitySystemComponent* ABECharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UTeamMemberComponent* ABECharacter::GetTeamMemberComponent() const
{
	auto* PS{ GetPlayerState() };

	return PS ? UTeamFunctionLibrary::GetTeamMemberComponentFromActor(PS) : nullptr;
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
