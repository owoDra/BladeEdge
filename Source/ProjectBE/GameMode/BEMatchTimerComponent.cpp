// Copyright (C) 2024 owoDra

#include "BEMatchTimerComponent.h"

#include "ProjectBELogs.h"

// Engine Features
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"

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

		SetTimerInfo({ EMatchTimerState::Countdown, GetMatchTimerState(), ServerTimeSeconds + Duration });
	}
}

void UBEMatchTimerComponent::StartCountup(double ElapsedTime)
{
	if (HasAuthority())
	{
		auto ServerTimeSeconds{ GetGameStateChecked<AGameStateBase>()->GetServerWorldTimeSeconds() };

		SetTimerInfo({ EMatchTimerState::CountUp, GetMatchTimerState(), ServerTimeSeconds - ElapsedTime });
	}
}

bool UBEMatchTimerComponent::Pause()
{
	if (HasAuthority() && GetMatchTimerState() != EMatchTimerState::Stop)
	{
		auto ServerTimeSeconds{ GetGameStateChecked<AGameStateBase>()->GetServerWorldTimeSeconds() };

		SetTimerInfo({ EMatchTimerState::Stop, GetMatchTimerState(), GetMatchTime() });
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

	TimerInfo = InTimerInfo;

	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, TimerInfo, this);

	HandleTimerInfoChange();
}

void UBEMatchTimerComponent::OnRep_TimerInfo()
{
	HandleTimerInfoChange();
}

void UBEMatchTimerComponent::HandleTimerInfoChange()
{
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


// Game Mode Option

bool UBEMatchTimerComponent::InitializeFromGameModeOption()
{
	if (!HasAuthority())
	{
		return false;
	}

	auto* GameMode{ GetWorld()->GetAuthGameMode() };
	if (ensure(GameMode))
	{
		auto OptionString{ GameMode->OptionsString };

		UE_LOG(LogBE_MatchTimer, Log, TEXT("Initialize MatchTimer From Game Mode Option"));

		EMatchTimerState NewState;
		EMatchTimerState NewLastState;
		double NewTime;

		auto HasAllOption
		{  
			UGameplayStatics::HasOption(OptionString, UBEMatchTimerComponent::NAME_MatchTimerTypeOptionKey) &&
			UGameplayStatics::HasOption(OptionString, UBEMatchTimerComponent::NAME_MatchTimerLastTypeOptionKey) &&
			UGameplayStatics::HasOption(OptionString, UBEMatchTimerComponent::NAME_MatchTimerTimeOptionKey)
		};

		if (HasAllOption)
		{
			{
				const auto OptionValue{ UGameplayStatics::ParseOption(OptionString, UBEMatchTimerComponent::NAME_MatchTimerTypeOptionKey) };
				NewState = static_cast<EMatchTimerState>(FCString::Atoi(*OptionValue));

				UE_LOG(LogBE_MatchTimer, Log, TEXT("| MatchTimerType: %s"), *StaticEnum<EMatchTimerState>()->GetDisplayValueAsText(NewState).ToString());
			}

			{
				const auto OptionValue{ UGameplayStatics::ParseOption(OptionString, UBEMatchTimerComponent::NAME_MatchTimerLastTypeOptionKey) };
				NewLastState = static_cast<EMatchTimerState>(FCString::Atoi(*OptionValue));

				UE_LOG(LogBE_MatchTimer, Log, TEXT("| MatchTimerLastType: %s"), *StaticEnum<EMatchTimerState>()->GetDisplayValueAsText(NewLastState).ToString());
			}

			{
				const auto OptionValue{ UGameplayStatics::ParseOption(OptionString, UBEMatchTimerComponent::NAME_MatchTimerTimeOptionKey) };
				NewTime = FCString::Atof(*OptionValue);

				UE_LOG(LogBE_MatchTimer, Log, TEXT("| MatchTimerTime: %f"), NewTime);
			}

			auto ServerTimeSeconds{ GetGameStateChecked<AGameStateBase>()->GetServerWorldTimeSeconds() };

			switch (NewState)
			{
			case EMatchTimerState::Countdown:
				SetTimerInfo({ NewState, NewLastState, ServerTimeSeconds + NewTime });
				break;
			case EMatchTimerState::CountUp:
				SetTimerInfo({ NewState, NewLastState, NewTime - ServerTimeSeconds });
				break;
			case EMatchTimerState::Stop:
				SetTimerInfo({ NewState, NewLastState, NewTime });
				break;
			}

			return true;
		}
		else
		{
			UE_LOG(LogBE_MatchTimer, Log, TEXT("| No Option"));
		}
	}

	return false;
}

FString UBEMatchTimerComponent::ConstructGameModeOption() const
{
	return FString::Printf(TEXT("?%s=%d?%s=%d?%s=%f")
		, *UBEMatchTimerComponent::NAME_MatchTimerTypeOptionKey
		, static_cast<int32>(GetMatchTimerState())
		, *UBEMatchTimerComponent::NAME_MatchTimerLastTypeOptionKey
		, static_cast<int32>(GetLastMatchTimerState())
		, *UBEMatchTimerComponent::NAME_MatchTimerTimeOptionKey
		, GetMatchTime());
}


// Utilities

UBEMatchTimerComponent* UBEMatchTimerComponent::FindMatchTimerComponent(AGameStateBase* GameState)
{
	return GameState ? GameState->FindComponentByClass<UBEMatchTimerComponent>() : nullptr;
}
