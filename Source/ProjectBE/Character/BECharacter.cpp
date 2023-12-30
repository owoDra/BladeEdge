// Copyright (C) 2023 owoDra

#include "BECharacter.h"

#include "GAEAbilitySystemComponent.h"

#include "HealthComponent.h"

#include "EquipmentManagerComponent.h"

#include "TeamFunctionLibrary.h"
#include "TeamMemberComponent.h"

#include "GameFramework/PlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BECharacter)


ABECharacter::ABECharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UGAEAbilitySystemComponent>(this, TEXT("AbilitySystem"));
	HealthComponent = ObjectInitializer.CreateDefaultSubobject<UHealthComponent>(this, TEXT("Health"));
	EquipmentManagerComponent = ObjectInitializer.CreateDefaultSubobject<UEquipmentManagerComponent>(this, TEXT("EquipmentManager"));
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
