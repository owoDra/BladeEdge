// Copyright (C) 2024 owoDra

#pragma once

#include "GameplayTask.h"

#include "BETask_ObservePlayerCount.generated.h"


/**
 * セッション内のプレイヤー数を監視するタスク
 */
UCLASS()
class UBETask_ObservePlayerCount : public UGameplayTask
{
	GENERATED_BODY()
public:
	UBETask_ObservePlayerCount(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTaskObservePlayerCountDelegate, bool, bEnough);

	UPROPERTY(BlueprintAssignable)
	FTaskObservePlayerCountDelegate OnChanged;

protected:
	mutable TWeakObjectPtr<AGameModeBase> GameMode{ nullptr };

	int32 RequiredPlayers{ 0 };
	bool bEnough{ false };
	float Interval{ 1.0f };

	FTimerHandle TimerHandle;

public:
	/** 
	 * セッション内のプレイヤー数を監視するタスク
	 */
	UFUNCTION(BlueprintCallable, Category = "GameplayTasks", meta = (AdvancedDisplay = "TaskOwner, Priority", DefaultToSelf = "TaskOwner", BlueprintInternalUseOnly = "TRUE"))
	static UBETask_ObservePlayerCount* TaskObservePlayerCount(
		TScriptInterface<IGameplayTaskOwnerInterface> TaskOwner
		, int32 InRequiredPlayers
		, float CheckInterval = 1.0f
		, bool bDefaultEnough = true
		, int32 Priority = 192);

public:
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	void HandleCheck();
	void HandleChanged();

	int32 GetNumPlayers() const;
};
