// Copyright (C) 2024 owoDra

#pragma once

#include "GameplayTask.h"

#include "BETask_WaitOneFrame.generated.h"


UCLASS(MinimalAPI)
class UBETask_WaitOneFrame : public UGameplayTask
{
	GENERATED_BODY()
public:
	UBETask_WaitOneFrame(const FObjectInitializer& ObjectInitializer);

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTaskWaitOneFrameDelegate);
	UPROPERTY(BlueprintAssignable)
	FTaskWaitOneFrameDelegate OnFinish;

protected:
	FTimerHandle TimerHandle;

public:
	/** 
	 * Wait one frame. This is functionally the same as a standard Delay node. 
	 */
	UFUNCTION(BlueprintCallable, Category = "GameplayTasks", meta = (AdvancedDisplay = "TaskOwner, Priority", DefaultToSelf = "TaskOwner", BlueprintInternalUseOnly = "TRUE"))
	static UBETask_WaitOneFrame* TaskWaitOneFrame(TScriptInterface<IGameplayTaskOwnerInterface> TaskOwner, const uint8 Priority = 192);

public:
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	void OnTimeFinish();

};
