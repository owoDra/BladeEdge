// Copyright (C) 2023 owoDra

#include "BEGameplayAbility_Jump.h"

#include "GameplayTag/GAETags_Ability.h"

#include "GameFramework/Character.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEGameplayAbility_Jump)


UBEGameplayAbility_Jump::UBEGameplayAbility_Jump(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalOnly;

	AbilityTags.AddTag(TAG_Ability_Type_Action);
}


void UBEGameplayAbility_Jump::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (auto* Character{ Cast<ACharacter>(ActorInfo->AvatarActor) })
	{
		Character->JumpMaxCount = MaxJumpableCount;
	}
}

void UBEGameplayAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CharacterJumpStart();
}

void UBEGameplayAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	CharacterJumpStop();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


void UBEGameplayAbility_Jump::CharacterJumpStart()
{
	if (auto* Character{ GetCharacter() })
	{
		if (Character->IsLocallyControlled())
		{
			Character->Jump();
		}
	}
}

void UBEGameplayAbility_Jump::CharacterJumpStop()
{
	if (auto* Character{ GetCharacter() })
	{
		if (Character->IsLocallyControlled())
		{
			Character->StopJumping();
		}
	}
}
