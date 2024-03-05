// Copyright (C) 2024 owoDra

#pragma once

#include "BESpawningManagerComponent.h"

#include "BESpawningManagerComponent_RandomUnique.generated.h"


/**
 * マップ内のランダムな一意の位置にプレイヤーをスポーンさせるスポーンマネージャー
 */
UCLASS(BlueprintType)
class PROJECTBE_API UBESpawningManagerComponent_RandomUnique : public UBESpawningManagerComponent
{
	GENERATED_BODY()
public:
	UBESpawningManagerComponent_RandomUnique(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	/////////////////////////////////////////////////////////////////
	// Choose Player Start
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Spawn")
	float ChosenListResetTimerInterval{ 5.0f };

	UPROPERTY(Transient)
	TSet<TWeakObjectPtr<APlayerStart>> ChosenPlayerStarts;

	UPROPERTY(Transient)
	FTimerHandle ChosenListResetTimerHandle;

protected:
	virtual AActor* OnChoosePlayerStart(AController* Player, TArray<APlayerStart*>& PlayerStarts) override;

	void SetChosenListResetTimer();
	virtual void HandleChosenListReset();

};
