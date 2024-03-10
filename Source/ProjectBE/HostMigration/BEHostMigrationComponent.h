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

	//
	// InitState に登録するための機能名
	//
	inline static const FName NAME_ActorFeatureName{ TEXTVIEW("HostMigration") };

	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }

	/////////////////////////////////////////////////////////////////
	// Initialization
protected:
	FDelegateHandle DisconnectHandle;

public:
	virtual void InitializeComponent() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	void HandleDisconnect(UWorld* InWorld, UNetDriver* NetDriver);


	/////////////////////////////////////////////////////////////////
	// Request
protected:
	/**
	 * ホストマイグレーションのための新規レベル開始のためのURLを作成する
	 */
	virtual FString CreateNewURL(UWorld* InWorld, UNetDriver* NetDriver) const;

	void SendHostMigrationRequest(UWorld* InWorld, const FString& NewURL);

};
