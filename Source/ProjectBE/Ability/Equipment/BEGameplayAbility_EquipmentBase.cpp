// Copyright (C) 2024 owoDra

#include "BEGameplayAbility_EquipmentBase.h"

#include "GameplayTag/BETags_Message.h"
#include "GameplayTag/BETags_Flag.h"

#include "AbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEGameplayAbility_EquipmentBase)


UBEGameplayAbility_EquipmentBase::UBEGameplayAbility_EquipmentBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	ActivationMethod = EAbilityActivationMethod::OnInputTriggered;
	ActivationPolicy = EAbilityActivationPolicy::Default;

	ActivationMessageTag = FGameplayTag::EmptyTag;
	bActivationMessageLocallyOnly = true;

	bUseCooldown = true;
	CooltimeOverride = 2.0f;
	CooldownMessageTag = FGameplayTag::EmptyTag;

	AbilityTags.AddTag(TAG_Flag_EquipmentUseBlocked);
	ActivationBlockedTags.AddTag(TAG_Flag_EquipmentUseBlocked);
}


void UBEGameplayAbility_EquipmentBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	BindTargetDataReadyCallback(Handle, ActivationInfo);

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UBEGameplayAbility_EquipmentBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsEndAbilityValid(Handle, ActorInfo))
	{
		if (ScopeLockCount > 0)
		{
			WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &ThisClass::EndAbility, Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled));
			return;
		}

		UnbindTargetDataReadyCallback(Handle, ActivationInfo);

		Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	}
}


// Target Data

void UBEGameplayAbility_EquipmentBase::BindTargetDataReadyCallback(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (!bShouldCallbackTargetDataReady)
	{
		return;
	}

	auto* ASC{ CurrentActorInfo->AbilitySystemComponent.Get() };
	check(ASC);

	TargetDataReadyCallbackDelegateHandle = ASC->AbilityTargetDataSetDelegate(Handle, ActivationInfo.GetActivationPredictionKey()).AddUObject(this, &ThisClass::HandleTargetDataReady);
}

void UBEGameplayAbility_EquipmentBase::UnbindTargetDataReadyCallback(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (!bShouldCallbackTargetDataReady)
	{
		return;
	}

	auto* ASC{ CurrentActorInfo->AbilitySystemComponent.Get() };
	check(ASC);

	ASC->AbilityTargetDataSetDelegate(Handle, ActivationInfo.GetActivationPredictionKey()).Remove(TargetDataReadyCallbackDelegateHandle);
	ASC->ConsumeClientReplicatedTargetData(Handle, ActivationInfo.GetActivationPredictionKey());
}


void UBEGameplayAbility_EquipmentBase::HandleTargetDataReady(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag)
{
	auto* ASC{ CurrentActorInfo->AbilitySystemComponent.Get() };
	check(ASC);

	auto ScopedPrediction{ FScopedPredictionWindow(ASC) };

	// ゲームコードへのコールバックによって、ターゲットデータが無効化されることがないように、ターゲットデータの所有権を取得する。

	auto LocalTargetDataHandle{ FGameplayAbilityTargetDataHandle(MoveTemp(const_cast<FGameplayAbilityTargetDataHandle&>(InData))) };

	// 必要であればサーバーに通知

	const auto bShouldNotifyServer{ CurrentActorInfo->IsLocallyControlled() && !CurrentActorInfo->IsNetAuthority() };
	if (bShouldNotifyServer)
	{
		ASC->CallServerSetReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey(), LocalTargetDataHandle, ApplicationTag, ASC->ScopedPredictionKey);
	}

	OnTargetDataReadyNative(LocalTargetDataHandle);
	OnTargetDataReady(LocalTargetDataHandle);

	ASC->ConsumeClientReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey());
}

void UBEGameplayAbility_EquipmentBase::SendTargetData(const FGameplayAbilityTargetDataHandle& InTargetData)
{
	if (!bShouldCallbackTargetDataReady)
	{
		return;
	}

	if (IsLocallyControlled())
	{
		HandleTargetDataReady(InTargetData, FGameplayTag());
	}
}
