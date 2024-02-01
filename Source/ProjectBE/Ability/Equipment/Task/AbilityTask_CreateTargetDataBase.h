// Copyright (C) 2024 owoDra

#pragma once

#include "Abilities/Tasks/AbilityTask.h"

#include "AbilityTask_CreateTargetDataBase.generated.h"


/**
 * ターゲットデータの作成を知らせるデリゲート
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTaskTargetDataCreatedDelegate, const FGameplayAbilityTargetDataHandle&, InTargetData);

/**
 * ターゲットデータが作成されなかったことを知らせるデリゲート
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTaskTargetDataNotCreatedDelegate);


/**
 * ターゲットデータを作成するためののアビリティタスクのベースクラス
 */
UCLASS(Abstract)
class PROJECTBE_API UAbilityTask_CreateTargetDataBase : public UAbilityTask
{
	GENERATED_BODY()
public:
	UAbilityTask_CreateTargetDataBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

private:
	UPROPERTY(BlueprintAssignable, meta = (AllowPrivateAccess = true))
	FTaskTargetDataCreatedDelegate OnCreated;

	UPROPERTY(BlueprintAssignable, meta = (AllowPrivateAccess = true))
	FTaskTargetDataNotCreatedDelegate OnNotCreated;

protected:
	/**
	 * ターゲットデータが作成されたことを知らせる
	 */
	void HandleTargetDataCreated(const FGameplayAbilityTargetDataHandle& InTargetData, bool bShouldFinishTask = true);

	/**
	 * ターゲットデータが作成されなかったことを知らせる
	 */
	void HandleTargetDataNotCreated();

};
