// Copyright (C) 2024 owoDra

#pragma once

#include "Component/GFCGameStateComponent.h"

#include "BEHostMigrationComponent.generated.h"


/**
 * 対戦中のマッチにおけるホストとの通信状況を監視し、切断されてしまった際にホストマイグレーションのための準備を行うコンポーネント
 */
UCLASS(BlueprintType)
class PROJECTBE_API UBEHostMigrationComponent : public UGFCGameStateComponent
{
	GENERATED_BODY()
public:
	UBEHostMigrationComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/////////////////////////////////////////////////////////////////
	// Initialization
protected:
	FDelegateHandle DisconnectHandle;

public:
	virtual void InitializeComponent() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	virtual void HandleDisconnect(UWorld* InWorld, UNetDriver* NetDriver);

};
