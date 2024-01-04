// Copyright (C) 2023 owoDra

#include "BEGameplayAbility_Dodge.h"

// Game Ability Extension
#include "GameplayTag/GAETags_Ability.h"

// Game Character: Locomotion Addon
#include "LocomotionComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEGameplayAbility_Dodge)


UBEGameplayAbility_Dodge::UBEGameplayAbility_Dodge(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	AbilityTags.AddTag(TAG_Ability_Type_Action);
}


void UBEGameplayAbility_Dodge::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	LocomotionComponent = Cast<ULocomotionComponent>(ActorInfo->MovementComponent);
}

bool UBEGameplayAbility_Dodge::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return LocomotionComponent.IsValid();
}

void UBEGameplayAbility_Dodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (ActorInfo->IsLocallyControlled())
	{
		ProcessSendMessage(ActorInfo->OwnerActor.Get(), RootMotionDuration + AbilityCooltime);

		StartDash();
	}
	else if (ActorInfo->IsNetAuthority())
	{
		StartDash();
	}
}


void UBEGameplayAbility_Dodge::StartDash()
{
	auto Direction{ FVector::ZeroVector };
	auto Angle{ 0.0f };

	GetDesiredDirection(Direction, Angle);

	ProcessExecuteGameplayCue(Direction);

	ProcessRootMotion(Direction, RootMotionStrength, RootMotionDuration);

	if (auto* Montage{ GetDesiredMontage(Angle) })
	{
		ProcessPlayMontage(Montage);
	}
}

void UBEGameplayAbility_Dodge::GetDesiredDirection(FVector& OutDirection, float& OutAngle) const
{
	if (ensure(LocomotionComponent.IsValid()))
	{
		auto MoveRotation{ FRotator::ZeroRotator };
		const auto ActorRotation{ LocomotionComponent->GetLocomotionState().Rotation };	
		const auto bHasInput{ LocomotionComponent->GetLocomotionState().bHasInput };

		if (bHasInput)
		{
			OutDirection = LocomotionComponent->GetInputDirection();

			MoveRotation = OutDirection.Rotation();
		}
		else
		{
			MoveRotation = LocomotionComponent->GetViewState().Rotation + FRotator(0.0f, 180.0f, 0.0f);

			OutDirection = MoveRotation.Vector();
		}

		MoveRotation -= ActorRotation;
		MoveRotation.Normalize();

		OutAngle = MoveRotation.Yaw;
	}
}

UAnimMontage* UBEGameplayAbility_Dodge::GetDesiredMontage(float Angle) const
{
	const auto AngleABS{ FMath::Abs(Angle) };

	if (AngleABS < 45.0f)
	{
		return DashMontage_F;
	}
	else if (AngleABS > 135.0f)
	{
		return DashMontage_B;
	}
	else if (Angle < 0.0f)
	{
		return DashMontage_L;
	}
	else
	{
		return DashMontage_R;
	}
}

void UBEGameplayAbility_Dodge::ProcessExecuteGameplayCue(FVector InDirection)
{
	if (GameplayCueTag.IsValid())
	{
		FGameplayCueParameters Param;
		Param.NormalizedMagnitude = RootMotionStrength;
		Param.RawMagnitude = RootMotionDuration;
		Param.Normal = InDirection;

		K2_ExecuteGameplayCueWithParams(GameplayCueTag, Param);
	}
}

void UBEGameplayAbility_Dodge::HandleRootMotionFinish()
{
	K2_CommitAbility();

	ProcessCooldown(AbilityCooltime);
}
