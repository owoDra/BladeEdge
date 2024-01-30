// Copyright (C) 2024 owoDra

#pragma once

#include "Subsystems/LocalPlayerSubsystem.h"

#include "Loadout/BELoadoutRequest.h"

#include "BELoadoutSubsystem.generated.h"

class UPlayerSaveSubsystem;
class UPlayerSave;
class UBEEquipmentItemData;


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
	// Equipment Item Data
#pragma region Equipment Item Data
protected:
	void ProcessItemDataFromId(FPrimaryAssetId AssetId, TObjectPtr<const UBEEquipmentItemData>& OutItemData);
	FPrimaryAssetId GetItemDataFromId(const UBEEquipmentItemData* ItemData) const;

protected:
	UPROPERTY(Transient) TObjectPtr<const UBEEquipmentItemData> FighterData;
	UPROPERTY(Transient) TObjectPtr<const UBEEquipmentItemData> WeaponData;
	UPROPERTY(Transient) TObjectPtr<const UBEEquipmentItemData> MainSkillData;
	UPROPERTY(Transient) TObjectPtr<const UBEEquipmentItemData> SubSkillData;
	UPROPERTY(Transient) TObjectPtr<const UBEEquipmentItemData> UltimateSkillData;

public:
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Loadout", meta = (AllowedTypes = "Fighter"))
	void SetFighterData(FPrimaryAssetId AssetId) { ProcessItemDataFromId(AssetId, FighterData); }

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Loadout", meta = (AllowedTypes = "Weapon"))
	void SetWeaponData(FPrimaryAssetId AssetId) { ProcessItemDataFromId(AssetId, WeaponData); }

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Loadout", meta = (AllowedTypes = "MainSkill"))
	void SetMainSkillData(FPrimaryAssetId AssetId) { ProcessItemDataFromId(AssetId, MainSkillData); }

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Loadout", meta = (AllowedTypes = "SubSkill"))
	void SetSubSkillData(FPrimaryAssetId AssetId) { ProcessItemDataFromId(AssetId, SubSkillData); }

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Loadout", meta = (AllowedTypes = "UltimateSkill"))
	void SetUltimateSkillData(FPrimaryAssetId AssetId) { ProcessItemDataFromId(AssetId, UltimateSkillData); }

public:
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Loadout")
	const UBEEquipmentItemData* GetFighterData() const { return FighterData; }

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Loadout")
	const UBEEquipmentItemData* GetWeaponData() const { return WeaponData; }

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Loadout")
	const UBEEquipmentItemData* GetMainSkillData() const { return MainSkillData; }

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Loadout")
	const UBEEquipmentItemData* GetSubSkillData() const { return SubSkillData; }

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Loadout")
	const UBEEquipmentItemData* GetUltimateSkillData() const { return UltimateSkillData; }

public:
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Loadout")
	FPrimaryAssetId GetFighterDataId() const { return GetItemDataFromId(FighterData); }

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Loadout")
	FPrimaryAssetId GetWeaponDataId() const { return GetItemDataFromId(WeaponData); }

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Loadout")
	FPrimaryAssetId GetMainSkillDataId() const { return GetItemDataFromId(MainSkillData); }

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Loadout")
	FPrimaryAssetId GetSubSkillDataId() const { return GetItemDataFromId(SubSkillData); }

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Loadout")
	FPrimaryAssetId GetUltimateSkillDataId() const { return GetItemDataFromId(UltimateSkillData); }

#pragma endregion


	//////////////////////////////////////////////////////////////////
	// Save
public:
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Save Game")
	virtual bool SaveLoadout();

protected:
	virtual void HandleLoadoutSaved(UPlayerSave* Save, bool bSuccess);


	//////////////////////////////////////////////////////////////////
	// Request
public:
	//
	// 可能ならばプレイヤーステートの保有する LoadoutComponent に現在のロードアウトの情報を送り
	// ネットワークへの反映をリクエストする。
	//
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Request")
	virtual bool SendLoadoutRequest() const;

};
