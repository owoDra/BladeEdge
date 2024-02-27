// Copyright (C) 2024 owoDra

#pragma once

#include "GameplayTask.h"

#include "BETask_WaitPlayers.generated.h"


/**
 * セッション内のプレイヤーの準備完了を待つタスク
 */
UCLASS()
class UBETask_WaitPlayers : public UGameplayTask
{
	GENERATED_BODY()
public:
	UBETask_WaitPlayers(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTaskWaitPlayersDelegate, int32, CurrentPlayers, int32, RequiedPlayers);

	UPROPERTY(BlueprintAssignable)
	FTaskWaitPlayersDelegate OnReady;
	
	UPROPERTY(BlueprintAssignable)
	FTaskWaitPlayersDelegate OnChanged;

protected:
	mutable TWeakObjectPtr<AGameStateBase> GameState{ nullptr };

	int32 RequiredPlayers{ 0 };
	int32 CurrentPlayers{ 0 };
	float Interval{ 2.0f };

	FTimerHandle TimerHandle;

public:
	/** 
	 * セッション内のプレイヤーの準備完了を待つタスク
	 */
	UFUNCTION(BlueprintCallable, Category = "GameplayTasks", meta = (AdvancedDisplay = "TaskOwner, Priority", DefaultToSelf = "TaskOwner", BlueprintInternalUseOnly = "TRUE"))
	static UBETask_WaitPlayers* TaskWaitPlayers(
		TScriptInterface<IGameplayTaskOwnerInterface> TaskOwner
		, int32 InRequiredPlayers
		, float CheckInterval = 2.0f
		, int32 Priority = 192);

public:
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	void HandleCheck();
	bool CheckHasAllReady(UWorld* InWorld);

	void HandleAllReady();
	void HandleChanged();

	AGameStateBase* GetGameState(UWorld* InWorld) const;
};
