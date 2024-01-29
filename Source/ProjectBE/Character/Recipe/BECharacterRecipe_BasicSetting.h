// Copyright (C) 2024 owoDra

#pragma once

#include "Recipe/CharacterRecipe.h"

#include "AbilitySet.h"

#include "BECharacterRecipe_BasicSetting.generated.h"

class UAbilityTagRelationshipMapping;
class UHealthData;
class ULocomotionData;
class UViewMode;


/**
 * キャラクターの基本アビリティの定義に使用するデータ
 */
USTRUCT(BlueprintType)
struct FBECharaRecipeBaseSetting_Ability
{
	GENERATED_BODY()
public:
	FBECharaRecipeBaseSetting_Ability() = default;

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<TSoftObjectPtr<UAbilitySet>> AbilitySets;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<const UAbilityTagRelationshipMapping> TagRelationshipMapping;

};


/**
 * キャラクターの基本体力の定義に使用するデータ
 */
USTRUCT(BlueprintType)
struct FBECharaRecipeBaseSetting_Health
{
	GENERATED_BODY()
public:
	FBECharaRecipeBaseSetting_Health() = default;

public:
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UHealthData> HealthData{ nullptr };

};


/**
 * キャラクターの基本移動能力の定義に使用するデータ
 */
USTRUCT(BlueprintType)
struct FBECharaRecipeBaseSetting_Locomotion
{
	GENERATED_BODY()
public:
	FBECharaRecipeBaseSetting_Locomotion() = default;

public:
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<ULocomotionData> LocomotionData{ nullptr };

};


/**
 * キャラクターの基本視点の定義に使用するデータ
 */
USTRUCT(BlueprintType)
struct FBECharaRecipeBaseSetting_View
{
	GENERATED_BODY()
public:
	FBECharaRecipeBaseSetting_View() = default;

public:
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UViewMode> ViewMode{ nullptr };

};


/**
 * このゲームにおけるキャラクターの基本情報のセットアップに使用するキャラクターレシピ
 */
UCLASS()
class UBECharacterRecipe_BasicSetting final : public UCharacterRecipe
{
	GENERATED_BODY()
public:
	UBECharacterRecipe_BasicSetting();

	///////////////////////////////////////////////////////////////
	// Ability
protected:
	UPROPERTY(EditDefaultsOnly, meta = (InlineEditConditionToggle))
	bool bApplyBasicAbilities{ false };

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bApplyBasicAbilities"))
	FBECharaRecipeBaseSetting_Ability BasicAbilities;


	///////////////////////////////////////////////////////////////
	// Health
protected:
	UPROPERTY(EditDefaultsOnly, meta = (InlineEditConditionToggle))
	bool bApplyBasicHealth{ false };

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bApplyBasicHealth"))
	FBECharaRecipeBaseSetting_Health BasicHealth;


	///////////////////////////////////////////////////////////////
	// Locomotion
protected:
	UPROPERTY(EditDefaultsOnly, meta = (InlineEditConditionToggle))
	bool bApplyBasicLocomotion{ false };

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bApplyBasicLocomotion"))
	FBECharaRecipeBaseSetting_Locomotion BasicLocomotion;


	///////////////////////////////////////////////////////////////
	// View
protected:
	UPROPERTY(EditDefaultsOnly, meta = (InlineEditConditionToggle))
	bool bApplyBasicView{ false };

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bApplyBasicView"))
	FBECharaRecipeBaseSetting_View BasicView;
	
protected:
	virtual void StartSetupNonInstanced_Implementation(FCharacterRecipePawnInfo Info) const override;

	void ServerOnlyProcess(APawn* Pawn) const;
	void ClientOnlyProcess(APawn* Pawn) const;
	void BothProcess(APawn* Pawn) const;

	void AbilityProcess(APawn* Pawn) const;
	void HealthProcess(APawn* Pawn) const;
	void LocomotionProcess(APawn* Pawn) const;
	void ViewProcess(APawn* Pawn) const;

};
