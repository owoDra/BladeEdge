// Copyright (C) 2024 owoDra

#include "BETask_WaitOneFrame.h"

#include "Engine/EngineTypes.h"
#include "TimerManager.h"
#include "Engine/World.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BETask_WaitOneFrame)


UBETask_WaitOneFrame::UBETask_WaitOneFrame(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


UBETask_WaitOneFrame* UBETask_WaitOneFrame::TaskWaitOneFrame(TScriptInterface<IGameplayTaskOwnerInterface> TaskOwner, const uint8 Priority)
{
	auto* MyTask{ NewTaskUninitialized<UBETask_WaitOneFrame>() };
	if (MyTask && TaskOwner.GetInterface() != nullptr)
	{
		MyTask->InitTask(*TaskOwner, Priority);
	}
	return MyTask;
}


void UBETask_WaitOneFrame::Activate()
{
	TimerHandle = GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UBETask_WaitOneFrame::OnTimeFinish);
}

void UBETask_WaitOneFrame::OnDestroy(bool bInOwnerFinished)
{
	if (auto* World{ GetWorld() })
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
	}

	Super::OnDestroy(bInOwnerFinished);
}

void UBETask_WaitOneFrame::OnTimeFinish()
{
	OnFinish.Broadcast();
	EndTask();
}
