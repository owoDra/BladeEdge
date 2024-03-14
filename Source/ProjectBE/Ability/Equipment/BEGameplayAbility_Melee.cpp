// Copyright (C) 2024 owoDra

#include "BEGameplayAbility_Melee.h"

#include "Ability/Equipment/Attribute/BECombatAttributeSet.h"
#include "GameplayTag/BETags_GameplayEvent.h"
#include "GameplayTag/BETags_Flag.h"
#include "GameplayTag/BETags_GameplayCue.h"

// BE Data Base
#include "BEDataBaseSubsystem.h"

// Game Item Core
#include "ItemData.h"

// Game Equipment Extension
#include "Equipment/Equipment.h"

// Game Locomotion: Human Addon
#include "LocomotionHumanNameStatics.h"

// Game Ability Extension
#include "GameplayTag/GAETags_SetByCaller.h"
#include "GAEAbilitySystemComponent.h"

#include "AbilitySystemGlobals.h"
#include "GameplayEffect.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEGameplayAbility_Melee)


UBEGameplayAbility_Melee::UBEGameplayAbility_Melee(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


void UBEGameplayAbility_Melee::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	const auto* World{ GetWorld() };
	const auto* DataBase{ World ? UGameInstance::GetSubsystem<UBEDataBaseSubsystem>(World->GetGameInstance()) : nullptr };

	if (ensure(DataBase))
	{
		MeleeTraceRadius	= DataBase->GetEquipmentParameter(DataBaseKey, AttackRadiusTag, MeleeTraceRadius);
		MeleeTraceDistance	= DataBase->GetEquipmentParameter(DataBaseKey, AttackDistanceTag, MeleeTraceDistance);

		MeleeAttackSpeed	= DataBase->GetEquipmentParameter(DataBaseKey, AttackSpeedTag, MeleeAttackSpeed);
		MeleeDamage			= DataBase->GetEquipmentParameter(DataBaseKey, AttackDamageTag, MeleeDamage);
		MeleeHeadMultiply	= DataBase->GetEquipmentParameter(DataBaseKey, HeadMultiplyTag, MeleeHeadMultiply);
	}

	Super::OnAvatarSet(ActorInfo, Spec);
}

void UBEGameplayAbility_Melee::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsEndAbilityValid(Handle, ActorInfo))
	{
		if (ScopeLockCount > 0)
		{
			WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &ThisClass::EndAbility, Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled));
			return;
		}

		ClearAttackTimer();

		Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	}
}

void UBEGameplayAbility_Melee::OnComboStart_Implementation()
{
	Step1_StartAttackTimer();
	Step2_PlayMontage();
}


// Melee

float UBEGameplayAbility_Melee::GetAttackSpeed() const
{
	auto Result{ 1.0f };

	if (auto* ASC{ GetAbilitySystemComponent() })
	{
		auto bFound{ false };
		Result = ASC->GetGameplayAttributeValue(UBECombatAttributeSet::GetAttackSpeedAttribute(), bFound);

		if (!bFound)
		{
			Result = 1.0f;
		}
	}

	return MeleeAttackSpeed * Result;
}

float UBEGameplayAbility_Melee::ComputeAttackTime_Implementation() const
{
	return MeleeAttackTime / GetAttackSpeed();
}

float UBEGameplayAbility_Melee::ComputeMeleeDamage_Implementation(const FHitResult& InHitResult) const
{
	const auto bIsHead{ InHitResult.BoneName == ULocomotionHumanNameStatics::HeadBoneName() };

	return bIsHead ? MeleeDamage * MeleeHeadMultiply : MeleeDamage;
}


// Parry

bool UBEGameplayAbility_Melee::CompareParryPower(const FGameplayAbilityTargetDataHandle& TargetData) const
{
	// 防御不可能な攻撃の場合

	if (bCannotParry)
	{
		return true;
	}

	// 最初のターゲットデータを取得し

	const auto* Data{ TargetData.Get(0) };
	if (!Data)
	{
		return false;
	}

	// Actor を取得

	auto Actors{ Data->GetActors() };
	if (Actors.IsEmpty())
	{
		return false;
	}

	auto* TargetActor{ Actors[0].Get() };
	auto* MyActor{ GetAvatarActorFromActorInfo() };

	if (!TargetActor || !MyActor)
	{
		return false;
	}

	// ASC を取得する

	auto* TargetASC{ UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor) };
	if (!TargetASC)
	{
		return false;
	}

	auto* MyASC{ GetAbilitySystemComponent() };

	// Parry 値を比較する

	const auto MyFlag{ MyASC->GetGameplayTagCount(TAG_Flag_Parry) };
	const auto TargetFlag{ TargetASC->GetGameplayTagCount(TAG_Flag_Parry) };
	const auto Delta{ TargetFlag - MyFlag };
	const auto AllowDamage_Power{ Delta < ParryPowerThreshold };

	if (AllowDamage_Power)
	{
		return true;	// Parry 値が上回っているならばダメージを与えられる
	}

	// 方向をチェック

	const auto TargetRotation{ TargetActor->GetActorRotation() };
	const auto MyRotation{ MyActor->GetActorRotation() + FRotator(0.0f, 180.0f, 0.0f) };
	const auto DeltaRotation{ TargetRotation - MyRotation };
	const auto AllowDamage_Angle{ FMath::Abs<float>(DeltaRotation.Yaw) >= ParryDirectionThreshold };

	if (AllowDamage_Angle)
	{
		return true;	// 角度的にパリー不可能ならばダメージを与えられる
	}

	return false;
}

void UBEGameplayAbility_Melee::NotifyParry(const FGameplayAbilityTargetDataHandle& TargetData)
{
	const auto* Data{ TargetData.Get(0) };
	if (!Data)
	{
		return;
	}

	const auto* HitResult{ Data->GetHitResult() };
	if (!HitResult)
	{
		return;
	}

	FGameplayCueParameters CueParameters;
	CueParameters.Location = (HitResult->bBlockingHit ? HitResult->ImpactPoint : HitResult->TraceEnd);
	CueParameters.Normal = HitResult->ImpactNormal;
	CueParameters.PhysicalMaterial = HitResult->PhysMaterial;

	K2_ExecuteGameplayCueWithParams(TAG_GameplayCue_Parry, CueParameters);
}


// Step1_StartTimer 

void UBEGameplayAbility_Melee::Step1_StartAttackTimer()
{
	if (auto* World{ GetWorld() })
	{
		World->GetTimerManager().SetTimer(AttackTimerHandle, this, &ThisClass::HandleAttackTimer, ComputeAttackTime(), false);
	}
}

void UBEGameplayAbility_Melee::ClearAttackTimer()
{
	if (AttackTimerHandle.IsValid())
	{
		if (auto* World{ GetWorld() })
		{
			World->GetTimerManager().ClearTimer(AttackTimerHandle);
		}
	}
}

void UBEGameplayAbility_Melee::HandleAttackTimer()
{
	AttackTimerHandle.Invalidate();

	K2_EndAbility();
}


// Step2_PlayMontage 

void UBEGameplayAbility_Melee::Step2_PlayMontage()
{
	HandlePlayMeleeMontage(MeleeMontage, FGameplayTagContainer(TAG_Event_Melee), GetAttackSpeed());
}

void UBEGameplayAbility_Melee::NotifyTargetNow()
{
	if (IsLocallyControlled())
	{
		Step3_Targeting();
	}
}


// Step3_Targeting

void UBEGameplayAbility_Melee::Step3_Targeting()
{
	HandleTargeting(MeleeTraceObjectTypes, MeleeTraceDuration, MeleeTraceRadius, MeleeTraceDistance);
}

void UBEGameplayAbility_Melee::OnTargetDataReadyNative(const FGameplayAbilityTargetDataHandle& TargetData)
{
	if (CompareParryPower(TargetData))
	{
		GiveDamage(TargetData);
	}
	else
	{
		NotifyParry(TargetData);
	}
}

void UBEGameplayAbility_Melee::GiveDamage(const FGameplayAbilityTargetDataHandle& TargetData)
{
	const auto* Data{ TargetData.Get(0) };
	if (!Data)
	{
		return;
	}

	const auto* HitResult{ Data->GetHitResult() };
	if (!HitResult)
	{
		return;
	}

	const auto Damage{ ComputeMeleeDamage(*HitResult) };
	auto SpecHandle{ MakeOutgoingGameplayEffectSpec(MeleeDamageGE, MeleeDirection) };
	auto* Spec{ SpecHandle.Data.Get() };
	if (Spec)
	{
		Spec->SetSetByCallerMagnitude(TAG_SetByCaller_Magnitude, Damage);
	}
	else
	{
		return;
	}

	K2_ApplyGameplayEffectSpecToTarget(SpecHandle, TargetData);
}
