// Copyright (C) 2024 owoDra

#pragma once

#include "GAEGameplayAbility.h"

#include "BEGameplayAbility_EquipmentBase.generated.h"


/**
 * このプロジェクトの装備用アビリティのベースクラス
 */
UCLASS(Abstract)
class PROJECTBE_API UBEGameplayAbility_EquipmentBase : public UGAEGameplayAbility
{
	GENERATED_BODY()
public:
	UBEGameplayAbility_EquipmentBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle
		, const FGameplayAbilityActorInfo* ActorInfo
		, const FGameplayAbilityActivationInfo ActivationInfo
		, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle
		, const FGameplayAbilityActorInfo* ActorInfo
		, const FGameplayAbilityActivationInfo ActivationInfo
		, bool bReplicateEndAbility
		, bool bWasCancelled) override;


protected:
	//
	// ターゲットデータのレプリケート時のコールバックを受け取るかどうか
	//
	UPROPERTY(AdvancedDisplay, EditDefaultsOnly, Category = "Targeting")
	bool bShouldCallbackTargetDataReady{ false };

	//
	// ターゲットデータがクライアントとサーバーで同期したことを知らせるデリゲート
	//
	FDelegateHandle TargetDataReadyCallbackDelegateHandle;

protected:
	void BindTargetDataReadyCallback(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActivationInfo ActivationInfo);
	void UnbindTargetDataReadyCallback(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActivationInfo ActivationInfo);

	void HandleTargetDataReady(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag);

	/**
	 * ターゲットデータがクライアントとサーバーで同期した時に実行される
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Targeting")
	void OnTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData);

	/**
	 * クライアントで作成されたターゲットデータをサーバーに送る
	 */
	UFUNCTION(BlueprintCallable, Category = "Targeting")
	void SendTargetData(const FGameplayAbilityTargetDataHandle& InTargetData);

};
