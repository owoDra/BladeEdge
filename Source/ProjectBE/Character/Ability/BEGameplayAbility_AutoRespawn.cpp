// Copyright (C) 2024 owoDra

#include "BEGameplayAbility_AutoRespawn.h"

#include "GameplayTag/GAETags_Ability.h"

#include "GameFramework/PlayerState.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "GameFramework/GameModeBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEGameplayAbility_AutoRespawn)


UBEGameplayAbility_AutoRespawn::UBEGameplayAbility_AutoRespawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	ActivationMethod = EAbilityActivationMethod::OnSpawn;

	AbilityTags.AddTag(TAG_Ability_Type_Misc);
}


void UBEGameplayAbility_AutoRespawn::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	auto* PlayerState{ Cast<APlayerState>(ActorInfo->OwnerActor) };

	if (ensure(PlayerState))
	{
		ListenPawnSet(PlayerState);

		if (auto* Pawn{ PlayerState->GetPawn() })
		{
			ListenPawnEndPlay(Pawn);
		}
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UBEGameplayAbility_AutoRespawn::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (auto* PlayerState{ GetActor<APlayerState>() })
	{
		UnlistenPawnSet(PlayerState);
	}

	if (CurrentPawn.IsValid())
	{
		UnlistenPawnEndPlay();
	}
}


void UBEGameplayAbility_AutoRespawn::ListenPawnSet(APlayerState* InPlayerState)
{
	FScriptDelegate NewDelegate;
	NewDelegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UBEGameplayAbility_AutoRespawn, HandlePawnSet));
	InPlayerState->OnPawnSet.Add(NewDelegate);
}

void UBEGameplayAbility_AutoRespawn::UnlistenPawnSet(APlayerState* InPlayerState)
{
	InPlayerState->OnPawnSet.RemoveAll(this);
}

void UBEGameplayAbility_AutoRespawn::HandlePawnSet(APlayerState* Player, APawn* NewPawn, APawn* OldPawn)
{
	if (NewPawn)
	{
		ListenPawnEndPlay(NewPawn);
	}
}


void UBEGameplayAbility_AutoRespawn::ListenPawnEndPlay(APawn* InPawn)
{
	CurrentPawn = InPawn;

	FScriptDelegate NewDelegate;
	NewDelegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UBEGameplayAbility_AutoRespawn, HandlePawnEndPlay));
	CurrentPawn->OnEndPlay.Add(NewDelegate);
}

void UBEGameplayAbility_AutoRespawn::UnlistenPawnEndPlay()
{
	CurrentPawn->OnEndPlay.RemoveAll(this);
	CurrentPawn.Reset();
}

void UBEGameplayAbility_AutoRespawn::HandlePawnEndPlay(AActor* Actor, EEndPlayReason::Type EndPlayReason)
{
	Respawn();
}


void UBEGameplayAbility_AutoRespawn::Respawn()
{
	if (K2_HasAuthority())
	{
		if (auto* PlayerState{ GetActor<APlayerState>() })
		{
			auto* Controller{ PlayerState->GetOwningController() };
			auto* GameMode{ GetWorld()->GetAuthGameMode() };

			if (GameMode && Controller)
			{
				GameMode->RestartPlayer(Controller);
			}
		}
	}
}
