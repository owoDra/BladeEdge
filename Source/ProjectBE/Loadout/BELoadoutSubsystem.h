// Copyright (C) 2024 owoDra

#pragma once

#include "Subsystems/LocalPlayerSubsystem.h"

#include "BELoadoutSubsystem.generated.h"

class UBEPlayerLoadoutSave;
class UPlayerSaveSubsystem;
class UItemData;


/**
 * プレイヤーのロードアウトを管理するサブシステム
 */
UCLASS()
class PROJECTBE_API UBELoadoutSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
public:
	UBELoadoutSubsystem() {}

	//////////////////////////////////////////////////////////////////
	// Initialization
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

protected:
	virtual void InitializeLoadout(UPlayerSaveSubsystem* SaveSubsystem);


	//////////////////////////////////////////////////////////////////
	// Skin
public:
	UFUNCTION(BlueprintCallable, Category = "Loadout|Skin")
	void SetFighterSkin(FPrimaryAssetId AssetId, FName FighterSkinName);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Loadout|Skin")
	FName GetFighterSkin(FPrimaryAssetId AssetId) const;


	UFUNCTION(BlueprintCallable, Category = "Loadout|Skin")
	void SetWeaponSkin(FPrimaryAssetId AssetId, FName WeaponSkinName);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Loadout|Skin")
	FName GetWeaponSkin(FPrimaryAssetId AssetId) const;


	//////////////////////////////////////////////////////////////////
	// Fighter Data
protected:
	UPROPERTY(Transient)
	TObjectPtr<const UItemData> Fighter;

public:
	UFUNCTION(BlueprintCallable, Category = "Loadout")
	void SetFighterData(FPrimaryAssetId AssetId);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Loadout")
	const UItemData* GetFighterData() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Loadout")
	FPrimaryAssetId GetFighterDataId() const;

protected:
	void SetFighterDataInternal(FPrimaryAssetId AssetId, const UItemData* Data);


	//////////////////////////////////////////////////////////////////
	// Weapon Data
protected:
	UPROPERTY(Transient)
	TObjectPtr<const UItemData> Weapon;

public:
	UFUNCTION(BlueprintCallable, Category = "Loadout")
	void SetWeaponData(FPrimaryAssetId AssetId);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Loadout")
	const UItemData* GetWeaponData() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Loadout")
	FPrimaryAssetId GetWeaponDataId() const;

protected:
	void SetWeaponDataInternal(FPrimaryAssetId AssetId, const UItemData* Data);


	//////////////////////////////////////////////////////////////////
	// MainSkill Data
protected:
	UPROPERTY(Transient)
	TObjectPtr<const UItemData> MainSkill;

public:
	UFUNCTION(BlueprintCallable, Category = "Loadout")
	void SetMainSkillData(FPrimaryAssetId AssetId);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Loadout")
	const UItemData* GetMainSkillData() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Loadout")
	FPrimaryAssetId GetMainSkillDataId() const;

protected:
	void SetMainSkillDataInternal(FPrimaryAssetId AssetId, const UItemData* Data);


	//////////////////////////////////////////////////////////////////
	// SubSkill Data
protected:
	UPROPERTY(Transient)
	TObjectPtr<const UItemData> SubSkill;

public:
	UFUNCTION(BlueprintCallable, Category = "Loadout")
	void SetSubSkillData(FPrimaryAssetId AssetId);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Loadout")
	const UItemData* GetSubSkillData() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Loadout")
	FPrimaryAssetId GetSubSkillDataId() const;

protected:
	void SetSubSkillDataInternal(FPrimaryAssetId AssetId, const UItemData* Data);


	//////////////////////////////////////////////////////////////////
	// UltimateSkill Data
protected:
	UPROPERTY(Transient)
	TObjectPtr<const UItemData> UltimateSkill;

public:
	UFUNCTION(BlueprintCallable, Category = "Loadout")
	void SetUltimateSkillData(FPrimaryAssetId AssetId);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Loadout")
	const UItemData* GetUltimateSkillData() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Loadout")
	FPrimaryAssetId GetUltimateSkillDataId() const;

protected:
	void SetUltimateSkillDataInternal(FPrimaryAssetId AssetId, const UItemData* Data);


	//////////////////////////////////////////////////////////////////
	// Utilities
protected:
	UBEPlayerLoadoutSave* GetSave() const;

	const UItemData* GetItemDataFromPrimaryAssetId(FPrimaryAssetId AssetId) const;

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Save Game")
	bool SaveLoadout();

};
