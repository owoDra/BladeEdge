// Copyright (C) 2024 owoDra

#pragma once

#include "Component/GFCPlayerStateComponent.h"

#include "Loadout/BELoadoutRequest.h"

#include "BELoadoutComponent.generated.h"


/**
 * ロードアウトが変更されたことを知らせるデリゲート
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FBELoadoutChangeDelegate, const UBELoadoutComponent*);


/**
 * ロードアウトデータをレプリケートするためのコンポーネント
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class PROJECTBE_API UBELoadoutComponent : public UGFCPlayerStateComponent
{
	GENERATED_BODY()
public:
	UBELoadoutComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	////////////////////////////////////////////////////////////////////////////////////
	// Init State
public:
	//
	// Function name used to add this component
	//
	inline static const FName NAME_ActorFeatureName{ TEXTVIEW("Loadout")};

public:
	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	////////////////////////////////////////////////////////////////////////////////////
	// Replication
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	////////////////////////////////////////////////////////////////////////////////////
	// Loadout Request
public:
	UPROPERTY(ReplicatedUsing = "OnRep_Loadout")
	FBELoadoutRequest Loadout;

protected:
	UFUNCTION()
	virtual void OnRep_Loadout();

public:
	UFUNCTION(BlueprintCallable, Category = "Loadout")
	virtual void LoadoutRequest(const FBELoadoutRequest& Request);

protected:
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerLoadoutRequest(const FBELoadoutRequest& Request);
	virtual bool ServerLoadoutRequest_Validate(const FBELoadoutRequest& Request);
	virtual void ServerLoadoutRequest_Implementation(const FBELoadoutRequest& Request);

	virtual void ReciveLoadoutRequest(const FBELoadoutRequest& Request);


	////////////////////////////////////////////////////////////////////////////////////
	// Loadout Asset Bundle
protected:
	virtual void LoadLoadoutAssetBundle(const FBELoadoutRequest& CurrentLoadout);

	virtual void HandleLoadoutAssetBundleLoaded();


	////////////////////////////////////////////////////////////////////////////////////
	// Loading Screen
protected:
	inline static const FName NAME_LoadoutAssetLoading{ TEXTVIEW("LoadoutAssetLoading") };

protected:
	void HandleShowLoadingLoadoutAssetScreen();
	void HandleHideLoadingLoadoutAssetScreen();


	////////////////////////////////////////////////////////////////////////////////////
	// Notify Change
public:
	FBELoadoutChangeDelegate OnLoadoutChange;

protected:
	virtual void BroadcastLoadoutChange();


	////////////////////////////////////////////////////////////////////////////////////
	// Utilities
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Component")
	static UBELoadoutComponent* FindBELoadoutComponent(const APlayerState* PlayerState);

};
