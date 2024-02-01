﻿// Copyright (C) 2024 owoDra

#pragma once

#include "Ability/Equipment/BEGameplayAbility_EquipmentBase.h"

#include "GameplayTagContainer.h"

#include "BEGameplayAbility_Melee.generated.h"

class UAbilitySystemComponent;


/**
 * 近接攻撃のためのアビリティベースクラス
 * 
 * Tips:
 *	コンボやターゲットデータのやり取りなどに必要な機能が実装されている
 */
UCLASS(Abstract)
class UBEGameplayAbility_Melee : public UBEGameplayAbility_EquipmentBase
{
	GENERATED_BODY()
public:
	UBEGameplayAbility_Melee(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual void PreActivate(
		const FGameplayAbilitySpecHandle Handle
		, const FGameplayAbilityActorInfo* ActorInfo
		, const FGameplayAbilityActivationInfo ActivationInfo
		, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate
		, const FGameplayEventData* TriggerEventData = nullptr);

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

	virtual bool DoesAbilitySatisfyTagRequirements(
		const UAbilitySystemComponent& AbilitySystemComponent
		, const FGameplayTagContainer* SourceTags
		, const FGameplayTagContainer* TargetTags
		, OUT FGameplayTagContainer* OptionalRelevantTags) const override;


	///////////////////////////////////////////////////////////
	// コンボ
protected:
	//
	// このアビリティがコンボの最初であるかどうか
	//
	UPROPERTY(EditDefaultsOnly, Category = "Combo")
	bool bIsComboRoot{ true };

	//
	// この派生コンボアビリティが起動するのに必要なタグ
	//
	UPROPERTY(EditDefaultsOnly, Category = "Combo", meta = (Categories = "Combo.Standby", EditCondition = "!bIsComboRoot"))
	FGameplayTag ComboStandbyTag;

	//
	// この派生コンボアビリティの実行するのに必要なタグ
	//
	UPROPERTY(EditDefaultsOnly, Category = "Combo", meta = (Categories = "Combo.Ready", EditCondition = "!bIsComboRoot"))
	FGameplayTag ComboReadyTag;

	//
	// この派生コンボアビリティを識別するためのタグ
	//
	UPROPERTY(EditDefaultsOnly, Category = "Combo", meta = (Categories = "Combo.Branch", EditCondition = "!bIsComboRoot"))
	FGameplayTag ComboBrunchTag;

	//
	// このコンボアビリティから派生するすべてのコンボアビリティのブランチタグ
	//
	UPROPERTY(EditDefaultsOnly, Category = "Combo", meta = (Categories = "Combo.Branch"))
	FGameplayTagContainer ChildComboBrunchTags;

private:
	//
	// ComboReadyTag が追加されるまで待つためのデリゲートハンドル
	//
	FDelegateHandle WaitComboReadyHandle;

protected:
	virtual void AddComboBrunchTagToAvatar();
	virtual void RemoveComboBrunchTagToAvatar();

protected:
	virtual void TryStartCombo();
	virtual void HandleComboReady();

	virtual void GameplayTagCallback(const FGameplayTag Tag, int32 NewCount);
	virtual void UnregisterGameplayTagCallback();

	UFUNCTION(BlueprintNativeEvent, Category = "Combo")
	void OnComboStart();
	virtual void OnComboStart_Implementation() {}

};
