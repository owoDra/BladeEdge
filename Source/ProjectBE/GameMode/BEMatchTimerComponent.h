// Copyright (C) 2024 owoDra

#pragma once

#include "Component/GFCGameStateComponent.h"

#include "BEMatchTimerComponent.generated.h"

class AGameStateBase;


/**
 * マッチタイマーのカウント状態
 */
UENUM(BlueprintType)
enum class EMatchTimerState : uint8
{
	Countdown,
	CountUp,
	Stop,
};


/**
 * マッチタイマーのカウント情報
 */
USTRUCT()
struct FMatchTimerInfo
{
	GENERATED_BODY()
public:
	FMatchTimerInfo() = default;
	FMatchTimerInfo(const EMatchTimerState& State, double Time)
		: State(State), Time(Time)
	{}

public:
	UPROPERTY()
	EMatchTimerState State{ EMatchTimerState::Stop };

	UPROPERTY(NotReplicated)
	EMatchTimerState LastState{ EMatchTimerState::Stop };

	//
	// ServerTimeSeconds をもとに現在のタイマー時間を計算するための数値。
	// State に応じて数値の意味は以下の通りに変化する。
	// 
	// Stop		 : 停止時の残り時間または経過時間 (直前のステートによって変化する。実際にUIなどに表示される数値と同じ。)
	// Countdown : カウントダウン終了予定時間の ServerTimeSeconds (Time - ServerTimeSeconds = タイマー時間)
	// Countup	 : カウント開始時の ServerTimeSeconds (ServerTimeSeconds - Time = タイマー時間)
	//
	UPROPERTY()
	double Time{ 0.0 };
};


/**
 * 対戦中のタイマーを管理するコンポーネント
 */
UCLASS(BlueprintType)
class PROJECTBE_API UBEMatchTimerComponent : public UGFCGameStateComponent
{
	GENERATED_BODY()
public:
	UBEMatchTimerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//
	// InitState に登録するための機能名
	//
	inline static const FName NAME_ActorFeatureName{ TEXTVIEW("MatchTimer") };

	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }

	/////////////////////////////////////////////////////////////////
	// Delegate
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMatchTimerChangedDelegate, EMatchTimerState, State, double, MatchTime);
	UPROPERTY(BlueprintAssignable)
	FMatchTimerChangedDelegate OnMatchTimerChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMatchTimerCountdownFinishDelegate);
	UPROPERTY(BlueprintAssignable)
	FMatchTimerCountdownFinishDelegate OnMatchTimerCountdownFinish;

protected:
	void BroadcastMatchTimerChanged();
	void BroadcastMatchTimerCountdownFinish();


	/////////////////////////////////////////////////////////////////
	// Match Timer
protected:
	//
	// 現在のタイマー情報
	// 
	// Tips:
	//	この変数はレプリケートされ、Client と Server で同じ数値となる。
	//
	UPROPERTY(ReplicatedUsing = "OnRep_TimerInfo")
	FMatchTimerInfo TimerInfo;

public:
	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category = "Match Timer")
	virtual void StartCountdown(double Duration);

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category = "Match Timer")
	virtual void StartCountup(double ElapsedTime = 0.0);

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category = "Match Timer")
	virtual bool Pause();

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category = "Match Timer")
	virtual bool Resume();

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category = "Match Timer")
	virtual bool ResumeCountdown();

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category = "Match Timer")
	virtual bool ResumeCountup();

protected:
	virtual void SetTimerInfo(FMatchTimerInfo InTimerInfo);

	UFUNCTION()
	void OnRep_TimerInfo(FMatchTimerInfo LastTimerInfo);

	virtual void HandleTimerInfoChange(FMatchTimerInfo LastTimerInfo);

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Match Timer")
	virtual double GetMatchTime() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Match Timer")
	virtual EMatchTimerState GetMatchTimerState() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Match Timer")
	virtual EMatchTimerState GetLastMatchTimerState() const;


	/////////////////////////////////////////////////////////////////
	// Countdown Timer
protected:
	//
	// カウントダウン終了を知らせるタイマーハンドル
	// 
	// Note:
	//	このタイマーは TimerInfo の State が "Countdown" の時のみ使用される。
	//  このタイマーは Client と Server どちらでも実行されるが、必ずしも同じタイミングで終了する保証はない。
	//
	FTimerHandle CountdownTimer;

protected:
	virtual void UpdateCountdownTimer();
	virtual void HandleCountdownFinish();

	
	/////////////////////////////////////////////////////////////////
	// Utilities
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Component")
	static UBEMatchTimerComponent* FindMatchTimerComponent(AGameStateBase* GameState);

};
