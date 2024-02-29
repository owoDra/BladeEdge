// Copyright (C) 2024 owoDra

#include "BEGameplayAbility_Melee.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEGameplayAbility_Melee)


UBEGameplayAbility_Melee::UBEGameplayAbility_Melee(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShouldCallbackTargetDataReady = true;
	bUseCooldown = false;
}


void UBEGameplayAbility_Melee::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	AddComboBrunchTagToAvatar();

	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
}

void UBEGameplayAbility_Melee::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	TryStartCombo();
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

		StopWaitReadyTimeoutTimer();
		RemoveComboBrunchTagToAvatar();

		Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	}
}

bool UBEGameplayAbility_Melee::DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::DoesAbilitySatisfyTagRequirements(AbilitySystemComponent, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	// 所有しているタグをキャッシュ

	auto& AbilitySystemGlobals{ UAbilitySystemGlobals::Get() };

	static FGameplayTagContainer AbilitySystemComponentTags;
	AbilitySystemComponentTags.Reset();

	AbilitySystemComponent.GetOwnedGameplayTags(AbilitySystemComponentTags);

	// ComboStandbyTag がない場合は許可しない

	if (!bIsComboRoot && ComboStandbyTag.IsValid())
	{
		if (!AbilitySystemComponentTags.HasTag(ComboStandbyTag))
		{
			const auto& MissingTag{ AbilitySystemGlobals.ActivateFailTagsMissingTag };

			if (OptionalRelevantTags && MissingTag.IsValid())
			{
				OptionalRelevantTags->AddTag(MissingTag);
			}

			return false;
		}
	}

	// ChildComboBrunchTags がある場合は許可しない

	if (AbilitySystemComponentTags.HasAny(ChildComboBrunchTags))
	{
		const auto& BlockedTag{ AbilitySystemGlobals.ActivateFailTagsBlockedTag };

		if (OptionalRelevantTags && BlockedTag.IsValid())
		{
			OptionalRelevantTags->AddTag(BlockedTag);
		}

		return false;
	}

	return true;
}


// コンボ

void UBEGameplayAbility_Melee::AddComboBrunchTagToAvatar()
{
	if (!bIsComboRoot)
	{
		if (auto* ASC{ GetAbilitySystemComponentFromActorInfo() })
		{
			ASC->SetLooseGameplayTagCount(ComboBrunchTag, 1);

			if (UAbilitySystemGlobals::Get().ShouldReplicateActivationOwnedTags())
			{
				ASC->SetReplicatedLooseGameplayTagCount(ComboBrunchTag, 1);
			}
		}
	}
}

void UBEGameplayAbility_Melee::RemoveComboBrunchTagToAvatar()
{
	if (!bIsComboRoot)
	{
		if (auto* ASC{ GetAbilitySystemComponentFromActorInfo() })
		{
			ASC->SetLooseGameplayTagCount(ComboBrunchTag, 0);

			if (UAbilitySystemGlobals::Get().ShouldReplicateActivationOwnedTags())
			{
				ASC->SetReplicatedLooseGameplayTagCount(ComboBrunchTag, 0);
			}
		}
	}
}


void UBEGameplayAbility_Melee::TryStartCombo()
{
	// コンボの最初だった場合は即コンボ開始

	if (bIsComboRoot)
	{
		HandleComboReady();
		return;
	}

	// タイムアウトタイマーを開始する

	StartWaitReadyTimeoutTimer();

	// コンボの最初出ない場合は ComboReadyTag があるか確認

	if (auto* ASC{ GetAbilitySystemComponentFromActorInfo() })
	{
		// すでに ComboReadyTag がある場合は即開始

		if (!ComboReadyTag.IsValid() || ASC->HasMatchingGameplayTag(ComboReadyTag))
		{
			HandleComboReady();
			return;
		}

		// ない場合は追加されるまで待つ

		if (WaitComboReadyHandle.IsValid())
		{
			UnregisterGameplayTagCallback();
		}

		WaitComboReadyHandle = ASC->RegisterGameplayTagEvent(ComboReadyTag).AddUObject(this, &UBEGameplayAbility_Melee::GameplayTagCallback);
	}
}

void UBEGameplayAbility_Melee::HandleComboReady()
{
	StopWaitReadyTimeoutTimer();

	OnComboStart();
}


void UBEGameplayAbility_Melee::StartWaitReadyTimeoutTimer()
{
	if (auto* World{ GetWorld()})
	{
		World->GetTimerManager().SetTimer(WaitComboReadyTimeOutHandle, this , &ThisClass::HandleWaitReadyTimeout, WaitReadyTimeoutTime, false);
	}
}

void UBEGameplayAbility_Melee::StopWaitReadyTimeoutTimer()
{
	if (WaitComboReadyTimeOutHandle.IsValid())
	{
		if (auto* World{ GetWorld() })
		{
			World->GetTimerManager().ClearTimer(WaitComboReadyTimeOutHandle);
		}
	}
}

void UBEGameplayAbility_Melee::HandleWaitReadyTimeout()
{
	WaitComboReadyTimeOutHandle.Invalidate();

	K2_CancelAbility();
}


void UBEGameplayAbility_Melee::GameplayTagCallback(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount == 1)
	{
		HandleComboReady();

		UnregisterGameplayTagCallback();
	}
}

void UBEGameplayAbility_Melee::UnregisterGameplayTagCallback()
{
	if (WaitComboReadyHandle.IsValid())
	{
		if (auto* ASC{ GetAbilitySystemComponentFromActorInfo() })
		{
			ASC->RegisterGameplayTagEvent(ComboReadyTag).Remove(WaitComboReadyHandle);
		}

		WaitComboReadyHandle.Reset();
	}
}
