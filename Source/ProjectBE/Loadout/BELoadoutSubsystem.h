// Copyright (C) 2024 owoDra

#pragma once

#include "Subsystems/LocalPlayerSubsystem.h"

#include "Type/BELoadoutRequest.h"

#include "BELoadoutSubsystem.generated.h"

class UPlayerSave;
class UPlayerSaveSubsystem;
class UBEPlayerLoadoutSave;
class UBEEquipmentItemData;


/**
 * 取得するスキンの種類
 */
UENUM(BlueprintType)
enum class EBESkinAccessorType : uint8
{
	// ファイターのスキンを取得・設定
	Fighter,

	// 武器のスキンを取得・設定
	Weapon
};


/**
 * プレイヤーのロードアウトを管理するサブシステム
 */
UCLASS()
class PROJECTBE_API UBELoadoutSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
public:
	UBELoadoutSubsystem() {}

	inline static const FName NAME_LoadoutLoading{ TEXTVIEW("LoadoutLoading")};
	inline static const FName NAME_LoadoutSaving{ TEXTVIEW("LoadoutSaving")};

	//////////////////////////////////////////////////////////////////
	// Initialization
protected:
	UPROPERTY()
	bool bLoadoutSaveReady{ false };

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

protected:
	virtual void InitializeLoadout(UPlayerSaveSubsystem* SaveSubsystem);

	virtual void HandleLoadoutSaveLoaded(UPlayerSave* Save, bool bSuccess);


	//////////////////////////////////////////////////////////////////
	// Loading Screen
protected:
	void HandleShowLoadingLoadoutScreen();
	void HandleHideLoadingLoadoutScreen();

	void HandleShowSavingLoadoutIndicator();
	void HandleHideSavingLoadoutIndicator();


	//////////////////////////////////////////////////////////////////
	// Skin
protected:
	UPROPERTY(Transient)
	TMap<FPrimaryAssetId, FName> FighterSkin;

	UPROPERTY(Transient)
	TMap<FPrimaryAssetId, FName> WeaponSkin;

public:
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Loadout|Skin")
	void SetSkin(
		EBESkinAccessorType Type
		, UPARAM(meta = (AllowedTypes = "Fighter, Weapon, MainSkill, SubSkill, UltimateSkill")) FPrimaryAssetId AssetId
		, FName SkinName);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Loadout|Skin")
	FName GetSkin(
		EBESkinAccessorType Type
		, UPARAM(meta = (AllowedTypes = "Fighter, Weapon, MainSkill, SubSkill, UltimateSkill")) FPrimaryAssetId AssetId) const;


	//////////////////////////////////////////////////////////////////
	// Equipment Item Data
protected:
	UPROPERTY(Transient)
	TMap<FGameplayTag, TObjectPtr<const UBEEquipmentItemData>> EquipmentItems;

public:
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Loadout")
	virtual void SetLoadoutItem(
		UPARAM(meta = (Categories = "Equipment.Slot")) FGameplayTag SlotTag
		, UPARAM(meta = (AllowedTypes = "Fighter, Weapon, MainSkill, SubSkill, UltimateSkill")) FPrimaryAssetId AssetId);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Loadout")
	virtual FPrimaryAssetId GetLoadoutItemAssetId(
		UPARAM(meta = (Categories = "Equipment.Slot")) FGameplayTag SlotTag);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Loadout")
	virtual const UBEEquipmentItemData* GetLoadoutItemData(
		UPARAM(meta = (Categories = "Equipment.Slot")) FGameplayTag SlotTag);

public:
	const UBEEquipmentItemData* ProcessEquipmentItemData(const FGameplayTag& SlotTag, const FPrimaryAssetId& AssetId) const;


	//////////////////////////////////////////////////////////////////
	// Save
public:
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Save Game")
	virtual bool SaveLoadout();

protected:
	virtual void HandleLoadoutSaved(UPlayerSave* Save, bool bSuccess);

};
