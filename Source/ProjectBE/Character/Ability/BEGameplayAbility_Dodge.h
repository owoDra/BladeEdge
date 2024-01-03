// Copyright (C) 2023 owoDra

#pragma once

#include "GAEGameplayAbility.h"

#include "BEGameplayAbility_Dodge.generated.h"

class ULocomotionComponent;
class UAnimMontage;


/**
 * キャラクターの回避アクションに必要な基本機能を追加した GameplayAbility クラス
 */
UCLASS(Abstract)
class PROJECTBE_API UBEGameplayAbility_Dodge : public UGAEGameplayAbility
{
	GENERATED_BODY()
public:
	UBEGameplayAbility_Dodge(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	TWeakObjectPtr<ULocomotionComponent> LocomotionComponent{ nullptr };

protected:
	virtual void OnAvatarSet(
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilitySpec& Spec) override;

	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags,
		const FGameplayTagContainer* TargetTags,
		FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Config|Montage")
	TObjectPtr<UAnimMontage> DashMontage_F{ nullptr };

	UPROPERTY(EditDefaultsOnly, Category = "Config|Montage")
	TObjectPtr<UAnimMontage> DashMontage_B{ nullptr };

	UPROPERTY(EditDefaultsOnly, Category = "Config|Montage")
	TObjectPtr<UAnimMontage> DashMontage_L{ nullptr };

	UPROPERTY(EditDefaultsOnly, Category = "Config|Montage")
	TObjectPtr<UAnimMontage> DashMontage_R{ nullptr };

	UPROPERTY(EditDefaultsOnly, Category = "Config|RootMotion")
	float RootMotionStrength{ 1000.0f };

	UPROPERTY(EditDefaultsOnly, Category = "Config|RootMotion")
	float RootMotionDuration{ 0.25f };

	UPROPERTY(EditDefaultsOnly, Category = "Config|Ability")
	float AbilityCooltime{ 0.3f };

protected:
	void StartDash();

	void GetDesiredDirection(FVector& OutDirection, float& OutAngle) const;

	UAnimMontage* GetDesiredMontage(float Angle) const;

protected:
	/**
	 * このイベントを使用して AnimMontage を再生する処理を実装する。
	 * 
	 * Tips:
	 *	無効な AnimMontage だった場合はこのイベントは実行されない。
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Dash")
	void ProcessPlayMontage(UAnimMontage* Montage);

	/**
	 * このイベントを使用して RootMotion による移動処理を実装する。
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Dash")
	void ProcessRootMotion(FVector Direction, float Strength, float Duration);

	/**
	 * RootMotion による移動処理が終了したらこの関数を実行する。
	 * 
	 * Tips:
	 *	アビリティコストの消費及びクールダウン処理の開始を行う。
	 */
	UFUNCTION(BlueprintCallable, Category = "Dash")
	void HandleRootMotionFinish();

	/**
	 * このイベントを使用してクールダウン処理実装する。
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Dash")
	void ProcessCooldown(float Cooltime);

};
