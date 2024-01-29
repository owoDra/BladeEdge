// Copyright (C) 2024 owoDra

#pragma once

#include "Component/GFCPlayerStateComponent.h"

#include "Loadout/Type/BELoadout.h"

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


	////////////////////////////////////////////////////////////////////////////////////
	// Replication
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	////////////////////////////////////////////////////////////////////////////////////
	// Loadout
private:
	UPROPERTY(ReplicatedUsing = "OnRep_Loadout")
	FBELoadout Loadout;

public:
	FBELoadoutChangeDelegate OnLoadoutChange;

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

	virtual void HandleLoadoutChange();

	void RebuildIndexMap();

public:
	/**
	 * 指定したスロットの ItemData を返す
	 */
	UFUNCTION(BlueprintCallable, Category = "Loadout", meta = (GameplayTagFilter = "Equipment.Slot"))
	const UBEEquipmentItemData* GetItemDataBySlot(FGameplayTag SlotTag) const;

	/**
	 * 指定したスロットのスキン名を返す
	 */
	UFUNCTION(BlueprintCallable, Category = "Loadout", meta = (GameplayTagFilter = "Equipment.Slot"))
	const FName GetSkinNameBySlot(FGameplayTag SlotTag) const;

	////////////////////////////////////////////////////////////////////////////////////
	// Utilities
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Component")
	static UBELoadoutComponent* FindBELoadoutComponent(const APlayerState* PlayerState);

};
