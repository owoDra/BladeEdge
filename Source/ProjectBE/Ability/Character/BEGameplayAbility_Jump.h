// Copyright (C) 2024 owoDra

#pragma once

#include "GAEGameplayAbility.h"

#include "BEGameplayAbility_Jump.generated.h"


/**
 * キャラクターのジャンプアクションに必要な基本機能を追加した GameplayAbility クラス
 */
UCLASS(Abstract)
class PROJECTBE_API UBEGameplayAbility_Jump : public UGAEGameplayAbility
{
	GENERATED_BODY()
public:
	UBEGameplayAbility_Jump(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Config", meta = (ClampMin = 0.00))
	int32 MaxJumpableCount{ 1 };

protected:
	virtual void OnAvatarSet(
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilitySpec& Spec) override;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled) override;

protected:
	UFUNCTION(BlueprintCallable, Category = "Jump")
	void CharacterJumpStart();

	UFUNCTION(BlueprintCallable, Category = "Jump")
	void CharacterJumpStop();

};
