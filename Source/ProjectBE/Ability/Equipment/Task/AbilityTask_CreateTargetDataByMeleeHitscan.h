// Copyright (C) 2024 owoDra

#pragma once

#include "AbilityTask_CreateTargetDataBase.h"

#include "AbilityTask_CreateTargetDataByMeleeHitscan.generated.h"

class UGameplayAbility;


/**
 * 近接攻撃による攻撃判定をヒットスキャンで行いターゲットデータを作成するタスク
 */
UCLASS()
class PROJECTBE_API UAbilityTask_CreateTargetDataByMeleeHitscan : public UAbilityTask_CreateTargetDataBase
{
	GENERATED_BODY()
public:
	UAbilityTask_CreateTargetDataByMeleeHitscan(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	////////////////////////////////////////////////////////////
	// Parameters
protected:
	//
	// トレースを行う時間
	//
	UPROPERTY(Transient)
	float Duration{ 0.0f };

	//
	// トレースの大きさ
	//
	UPROPERTY(Transient)
	float Radius{ 20.0f };

	//
	// トレースの最大距離
	//
	UPROPERTY(Transient)
	float Distance{ 300.0f };

	//
	// トレースが当たるオブジェクトタイプのリスト
	//
	UPROPERTY(Transient)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	//
	// トレースが無視するアクターのリスト
	//
	UPROPERTY(Transient)
	TArray<TObjectPtr<AActor>> ActorsToIgnore;

	//
	// トレースを行うチャンネル
	//
	UPROPERTY(Transient)
	TEnumAsByte<ECollisionChannel> CollisionChannel{ ECollisionChannel::ECC_Visibility };

	//
	// AvatarActor およびその子 Actor を無視するか
	//
	UPROPERTY(Transient)
	bool bIgnoreAvatarAndChildren{ true };

public:
	/**
	 * 近接攻撃による攻撃判定をヒットスキャンで行いターゲットデータを作成するタスク
	 * 
	 * @Param InObjectTypes					トレースが当たるオブジェクトタイプのリスト
	 * @Param InActorsToIgnore				トレースが無視するアクターのリスト
	 * @Param bInIgnoreAvatarAndChildren	AvatarActor およびその子 Actor を無視するか
	 * @Param InCollisionChannel			トレースを行うチャンネル
	 * @Param InDuration					トレースを行う時間
	 * @Param InRadius						トレースの大きさ
	 * @Param InDistance					トレースの最大距離
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, BlueprintInternalUseOnly, Category = "AbilityTask|TargetData", meta = (AutoCreateRefTerm = "InActorsToIgnore", DefaultToSelf = "OwningAbility", DisplayName = "Create Target Data By Melee Hitscan", HidePin = "OwningAbility"))
	static UAbilityTask_CreateTargetDataByMeleeHitscan* CreateCreateTargetDataByMeleeHitscan(
		UGameplayAbility* OwningAbility
		, const TArray<TEnumAsByte<EObjectTypeQuery>>& InObjectTypes
		, const TArray<AActor*>& InActorsToIgnore
		, bool bInIgnoreAvatarAndChildren = true
		, ECollisionChannel InCollisionChannel = ECollisionChannel::ECC_Visibility
		, float InDuration = 0.0f
		, float InRadius = 20.0f
		, float InDistance = 300.f);

public:
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

protected:
	bool TryCreateTargetData();

	/**
	 * AvatarActor とその ChildActors を ActorsToIgnore に追加する
	 */
	virtual void StoreAvatarToIgnoreList();


	///////////////////////////////////////////////////////
	// Repeat Process
#pragma region Repeat Process
protected:
	//
	// トレースを繰り返すためのタイマーハンドル
	//
	UPROPERTY(Transient)
	FTimerHandle RepeatTimerHandle;

	//
	// トレースの開始時間
	//
	UPROPERTY(Transient)
	float StartTime;

protected:
	/**
	 * タイマーを設定する。false を返した場合はタイマーの必要がないことを示す。
	 */
	bool RegisterTimerEvent();
	void UnregisterTimerEvent();
	
	void PerformTick();

private:
	void SetTimer(bool bInitalSet = false);

#pragma endregion


	///////////////////////////////////////////////////////
	// Trace
#pragma region Trace
protected:
	/**
	 * ライントレースを実行する
	 */
	virtual bool PerformLineTrace(FHitResult& OutResult);

	/**
	 * シリンダートレースを行う
	 */
	virtual bool PerformCylinderTraceTrace(FHitResult& OutResult);

	/**
	 * トレースの有効性を確かめるためのトレースを行う
	 */
	virtual bool PerformValidationLineTrace(const FHitResult& InHitResult);

protected:
	/**
	 * トレースの開始地点と終了地点を計算する
	 */
	virtual void CalcTraceStartEnd(FVector& OutStart, FVector& OutEnd) const;
	
#pragma endregion

};
