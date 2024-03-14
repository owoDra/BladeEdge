// Copyright (C) 2024 owoDra

#include "BEGameplayAbility_Combo.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEGameplayAbility_Combo)


UBEGameplayAbility_Combo::UBEGameplayAbility_Combo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShouldCallbackTargetDataReady = true;
	bUseCooldown = false;
}


void UBEGameplayAbility_Combo::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	AddComboBrunchTagToAvatar();

	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
}

void UBEGameplayAbility_Combo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	TryStartCombo();
}

void UBEGameplayAbility_Combo::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
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

bool UBEGameplayAbility_Combo::DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
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

void UBEGameplayAbility_Combo::AddComboBrunchTagToAvatar()
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

void UBEGameplayAbility_Combo::RemoveComboBrunchTagToAvatar()
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


void UBEGameplayAbility_Combo::TryStartCombo()
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

		WaitComboReadyHandle = ASC->RegisterGameplayTagEvent(ComboReadyTag).AddUObject(this, &UBEGameplayAbility_Combo::GameplayTagCallback);
	}
}

void UBEGameplayAbility_Combo::HandleComboReady()
{
	StopWaitReadyTimeoutTimer();

	OnComboStart();
}


void UBEGameplayAbility_Combo::StartWaitReadyTimeoutTimer()
{
	if (auto* World{ GetWorld()})
	{
		World->GetTimerManager().SetTimer(WaitComboReadyTimeOutHandle, this , &ThisClass::HandleWaitReadyTimeout, WaitReadyTimeoutTime, false);
	}
}

void UBEGameplayAbility_Combo::StopWaitReadyTimeoutTimer()
{
	if (WaitComboReadyTimeOutHandle.IsValid())
	{
		if (auto* World{ GetWorld() })
		{
			World->GetTimerManager().ClearTimer(WaitComboReadyTimeOutHandle);
		}
	}
}

void UBEGameplayAbility_Combo::HandleWaitReadyTimeout()
{
	WaitComboReadyTimeOutHandle.Invalidate();

	K2_CancelAbility();
}


void UBEGameplayAbility_Combo::GameplayTagCallback(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount == 1)
	{
		HandleComboReady();

		UnregisterGameplayTagCallback();
	}
}

void UBEGameplayAbility_Combo::UnregisterGameplayTagCallback()
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
