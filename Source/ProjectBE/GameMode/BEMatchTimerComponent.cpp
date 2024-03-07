// Copyright (C) 2024 owoDra

#include "BEMatchTimerComponent.h"

// Engine Features
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"
#include "GameFramework/GameStateBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEMatchTimerComponent)

////////////////////////////////////////////////////////////////////////////

UBEMatchTimerComponent::UBEMatchTimerComponent(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UBEMatchTimerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	Params.Condition = COND_None;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, TimerInfo, Params);
}


// Delegate

void UBEMatchTimerComponent::BroadcastMatchTimerChanged()
{
	OnMatchTimerChanged.Broadcast(GetMatchTimerState(), GetMatchTime());
}

void UBEMatchTimerComponent::BroadcastMatchTimerCountdownFinish()
{
	OnMatchTimerCountdownFinish.Broadcast();
}


// Match Timer

void UBEMatchTimerComponent::StartCountdown(double Duration)
{
	if (HasAuthority())
	{
		auto ServerTimeSeconds{ GetGameStateChecked<AGameStateBase>()->GetServerWorldTimeSeconds() };

		SetTimerInfo({ EMatchTimerState::Countdown, ServerTimeSeconds + Duration });
	}
}

void UBEMatchTimerComponent::StartCountup(double ElapsedTime)
{
	if (HasAuthority())
	{
		auto ServerTimeSeconds{ GetGameStateChecked<AGameStateBase>()->GetServerWorldTimeSeconds() };

		SetTimerInfo({ EMatchTimerState::CountUp, ServerTimeSeconds - ElapsedTime });
	}
}

bool UBEMatchTimerComponent::Pause()
{
	if (HasAuthority() && GetMatchTimerState() != EMatchTimerState::Stop)
	{
		auto ServerTimeSeconds{ GetGameStateChecked<AGameStateBase>()->GetServerWorldTimeSeconds() };

		SetTimerInfo({ EMatchTimerState::Stop, GetMatchTime() });
		return true;
	}

	return false;
}

bool UBEMatchTimerComponent::Resume()
{
	if (HasAuthority() && GetMatchTimerState() == EMatchTimerState::Stop && GetLastMatchTimerState() != EMatchTimerState::Stop)
	{
		auto ServerTimeSeconds{ GetGameStateChecked<AGameStateBase>()->GetServerWorldTimeSeconds() };

		switch (GetLastMatchTimerState())
		{
		case EMatchTimerState::Countdown:
			StartCountdown(GetMatchTime());
			return true;
			break;

		case EMatchTimerState::CountUp:
			StartCountup(GetMatchTime());
			return true;
			break;
		}
	}

	return false;
}

bool UBEMatchTimerComponent::ResumeCountdown()
{
	if (HasAuthority() && GetMatchTimerState() == EMatchTimerState::Stop)
	{
		StartCountdown(GetMatchTime());
		return true;
	}

	return false;
}

bool UBEMatchTimerComponent::ResumeCountup()
{
	if (HasAuthority() && GetMatchTimerState() == EMatchTimerState::Stop)
	{
		StartCountup(GetMatchTime());
		return true;
	}

	return false;
}


void UBEMatchTimerComponent::SetTimerInfo(FMatchTimerInfo InTimerInfo)
{
	check(HasAuthority());

	InTimerInfo.Time = FMath::Max(InTimerInfo.Time, 0.0);

	const auto LastTimerInfo{ TimerInfo };

	TimerInfo = InTimerInfo;

	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, TimerInfo, this);

	HandleTimerInfoChange(LastTimerInfo);
}

void UBEMatchTimerComponent::OnRep_TimerInfo(FMatchTimerInfo LastTimerInfo)
{
	HandleTimerInfoChange(LastTimerInfo);
}

void UBEMatchTimerComponent::HandleTimerInfoChange(FMatchTimerInfo LastTimerInfo)
{
	TimerInfo.LastState = LastTimerInfo.State;

	BroadcastMatchTimerChanged();

	UpdateCountdownTimer();
}

double UBEMatchTimerComponent::GetMatchTime() const
{
	auto ServerTimeSeconds{ GetGameStateChecked<AGameStateBase>()->GetServerWorldTimeSeconds() };

	switch (GetMatchTimerState())
	{
	case EMatchTimerState::Countdown:
		return FMath::Max(TimerInfo.Time - ServerTimeSeconds, 0.0);
		break;

	case EMatchTimerState::CountUp:
		return FMath::Max(ServerTimeSeconds - TimerInfo.Time, 0.0);
		break;

	default:
		return TimerInfo.Time;
		break;
	}
}

EMatchTimerState UBEMatchTimerComponent::GetMatchTimerState() const
{
	return TimerInfo.State;
}

EMatchTimerState UBEMatchTimerComponent::GetLastMatchTimerState() const
{
	return TimerInfo.LastState;
}


// World Timer

void UBEMatchTimerComponent::UpdateCountdownTimer()
{
	if (auto* World{ GetWorld() })
	{
		auto& TimerManager{ World->GetTimerManager() };

		switch (GetMatchTimerState())
		{
		case EMatchTimerState::Countdown:
			TimerManager.ClearTimer(CountdownTimer);
			if (GetMatchTime() > 0.0)
			{
				TimerManager.SetTimer(CountdownTimer, this, &ThisClass::HandleCountdownFinish, GetMatchTime(), false);
			}
			else
			{
				HandleCountdownFinish();
			}
			break;

		case EMatchTimerState::CountUp:
		case EMatchTimerState::Stop:
			TimerManager.ClearTimer(CountdownTimer);
			break;
		}
	}
}

void UBEMatchTimerComponent::HandleCountdownFinish()
{
	BroadcastMatchTimerCountdownFinish();
}


// Utilities

UBEMatchTimerComponent* UBEMatchTimerComponent::FindMatchTimerComponent(AGameStateBase* GameState)
{
	return GameState ? GameState->FindComponentByClass<UBEMatchTimerComponent>() : nullptr;
}
