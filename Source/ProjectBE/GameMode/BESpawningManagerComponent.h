// Copyright (C) 2024 owoDra

#pragma once

#include "Component/GFCGameStateComponent.h"

#include "BESpawningManagerComponent.generated.h"

class APlayerStart;
class AController;
class AActor;


/**
 * スタート地点の占有率。スタートポイントからスポーン可能かどうかの目安となる。
 */
enum class EPlayerStartLocationOccupancy
{
	Empty,
	Partial,
	Full
};


/**
 * ゲームモード内でのプレイヤーのスポーン/リスポーンを管理するをコンポーネント
 */
UCLASS(BlueprintType)
class PROJECTBE_API UBESpawningManagerComponent : public UGFCGameStateComponent
{
	GENERATED_BODY()
public:
	UBESpawningManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/////////////////////////////////////////////////////////////////
	// Initialization
public:
	virtual void InitializeComponent() override;

protected:
	virtual void OnLevelAdded(ULevel* InLevel, UWorld* InWorld);
	virtual void HandleOnActorSpawned(AActor* SpawnedActor);


	/////////////////////////////////////////////////////////////////
	// Cache Player Start
protected:
	UPROPERTY(Transient)
	TSet<TWeakObjectPtr<APlayerStart>> CachedPlayerStarts;

protected:
	virtual void CachePlayerStartsFromWorld(UWorld* InWorld);
	virtual void CachePlayerStartsFromLevel(ULevel* InLevel);
	virtual void CachePlayerStartFromSpecificActor(AActor* InActor);


	/////////////////////////////////////////////////////////////////
	// Choose Player Start
public:
	AActor* ChoosePlayerStart(AController* Player);
	
protected:
	virtual AActor* OnChoosePlayerStart(AController* Player, TArray<APlayerStart*>& PlayerStarts) { return nullptr; }


	/////////////////////////////////////////////////////////////////
	// Utilities
public:
	APlayerStart* GetFirstRandomUnoccupiedPlayerStart(AController* Controller, const TArray<APlayerStart*>& FoundStartPoints) const;
	EPlayerStartLocationOccupancy GetPlayerStartOccupancy(UWorld* World, const APlayerStart* PlayerStart, const AActor* TestActor) const;

};
