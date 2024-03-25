// Copyright (C) 2024 owoDra

#pragma once

#include "Ability/Equipment/BEGameplayAbility_Combo.h"

#include "BEGameplayAbility_Melee.generated.h"

class UAnimMontage;
class UGameplayEffect;


/**
 * 近接攻撃のためのアビリティベースクラス
 * 
 * Tips:
 *	基本的な攻撃判定処理とダメージ適用処理、アニメーション処理が含まれている
 */
UCLASS(Abstract)
class UBEGameplayAbility_Melee : public UBEGameplayAbility_Combo
{
	GENERATED_BODY()
public:
	UBEGameplayAbility_Melee(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle
		, const FGameplayAbilityActorInfo* ActorInfo
		, const FGameplayAbilityActivationInfo ActivationInfo
		, bool bReplicateEndAbility
		, bool bWasCancelled) override;

	virtual void OnComboStart_Implementation() override;


	///////////////////////////////////////////////////////////
	// Melee
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Melee|Trace")
	TArray<TEnumAsByte<EObjectTypeQuery>> MeleeTraceObjectTypes{ EObjectTypeQuery::ObjectTypeQuery7 };

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Melee|Trace")
	float MeleeTraceDuration{ 0.0f };

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Melee|Trace")
	float MeleeTraceRadius{ 25.0f };

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Melee|Trace")
	float MeleeTraceDistance{ 300.0f };

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Melee|Damage")
	TSubclassOf<UGameplayEffect> MeleeDamageGE{ nullptr };

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Melee|Damage")
	float MeleeDamage{ 30.0f };

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Melee|Damage")
	float MeleeHeadMultiply{ 1.5f };

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Melee")
	TObjectPtr<UAnimMontage> MeleeMontage{ nullptr };

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Melee")
	float MeleeAttackSpeed{ 1.0f };

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Melee")
	float MeleeAttackTime{ 1.0f };

	UPROPERTY(BlueprintReadWrite, Category = "Melee")
	float MeleeDirection{ 0.0f };

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataBase|Melee", meta = (Categories = "DataBase.Weapon.AttackRadius"))
	FGameplayTag AttackRadiusTag;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataBase|Melee", meta = (Categories = "DataBase.Weapon.AttackDistance"))
	FGameplayTag AttackDistanceTag;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataBase|Melee", meta = (Categories = "DataBase.Weapon.AttackSpeed"))
	FGameplayTag AttackSpeedTag;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataBase|Melee", meta = (Categories = "DataBase.Weapon.AttackDamage"))
	FGameplayTag AttackDamageTag;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "DataBase|Melee", meta = (Categories = "DataBase.Weapon.HeadMultiply"))
	FGameplayTag HeadMultiplyTag;

protected:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Melee")
	virtual float GetAttackSpeed() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, BlueprintPure, Category = "Melee")
	float ComputeAttackTime() const;
	virtual float ComputeAttackTime_Implementation() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, BlueprintPure, Category = "Melee")
	float ComputeMeleeDamage(const FHitResult& InHitResult) const;
	virtual float ComputeMeleeDamage_Implementation(const FHitResult& InHitResult) const;


	///////////////////////////////////////////////////////////
	// Step1_StartTimer 
protected:
	FTimerHandle AttackTimerHandle;

protected:
	virtual void Step1_StartAttackTimer();
	virtual void ClearAttackTimer();
	virtual void HandleAttackTimer();


	///////////////////////////////////////////////////////////
	// Step2_PlayMontage 
protected:
	virtual void Step2_PlayMontage();

	UFUNCTION(BlueprintImplementableEvent, Category = "Melee")
	void HandlePlayMeleeMontage(UAnimMontage* Montage, const FGameplayTagContainer& EventTags, float PlayRate);

	UFUNCTION(BlueprintCallable, Category = "Melee")
	virtual void NotifyTargetNow(float InMeleeDirection);


	///////////////////////////////////////////////////////////
	// Step3_Targeting
protected:
	virtual void Step3_Targeting(float InMeleeDirection);

	UFUNCTION(BlueprintImplementableEvent, Category = "Melee")
	void HandleTargeting(const TArray<TEnumAsByte<EObjectTypeQuery>>& Types, float Duration, float Radius, float Distance);

	virtual void OnTargetDataReadyNative(const FGameplayAbilityTargetDataHandle& TargetData) override;
	virtual void GiveDamage(const FGameplayAbilityTargetDataHandle& TargetData);

};
