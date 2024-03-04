// Copyright (C) 2024 owoDra

#pragma once

#include "AbilityLocomotionCharacter.h"
#include "AbilitySystemInterface.h"
#include "TeamMemberComponentInterface.h"
#include "EquipmentManagerComponentInterface.h"
#include "HealthComponentInterface.h"
#include "ContextEffectInterface.h"
#include "GameplayTag/GameplayTagStackInterface.h"


#include "BECharacter.generated.h"

class UCharacterInitStateComponent;
class UGAEAbilitySystemComponent;
class UHealthComponent;
class UEquipmentManagerComponent;
class UContextEffectComponent;


/** 
 * このプロジェクトにおける Character のベースクラス
 */
UCLASS(Blueprintable)
class PROJECTBE_API ABECharacter 
	: public AAbilityLocomotionCharacter
	, public IAbilitySystemInterface
	, public ITeamMemberComponentInterface
	, public IEquipmentManagerComponentInterface
	, public IHealthComponentInterface
	, public IContextEffectInterface
	, public IGameplayTagStackInterface
{
	GENERATED_BODY()
public:
	explicit ABECharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	/**
	 * このプロジェクトにおいて Character は以下のコンポーネントを使用するが、
	 * コンポーネントの一部は PlayerState 側が保有する。それらの詳細は下記に記されている。
	 * 
	 *	- AbilitySystemComponent
	 *	| [オーナー] Character
	 *	| [　理由　] 個々のキャラクターの能力を表現するため。
	 * 
	 *  - AbilityLocomotionSystem
	 *	| [オーナー] Character
	 *	| [　理由　] キャラクターに完全に依存するため。
	 * 
	 *	- HealthComponent
	 *	| [オーナー] Character
	 *	| [　理由　] 個々のキャラクターのヘルスを表現するため。
	 * 
	 *	- EquipmentManagerComponent
	 *	| [オーナー] Character
	 *	| [　理由　] 個々のキャラクターの装備を表現するため。
	 * 
	 *	- TeamMemberComponent
	 *	| [オーナー] PlayerState
	 *	| [　理由　] キャラクターを操作するプレイヤーのチームを表現するため。
	 * 
	 *	- AbilityPlayableComponent
	 *	| [オーナー] Character
	 *	| [　理由　] 個々のキャラクターの入力処理を行うため。
	 * 
	 *	- BEViewerComponent
	 *	| [オーナー] Character
	 *	| [　理由　] 個々のキャラクターの視点を表現するため。
	 * 
	 *	- ContextEffectComponent
	 *	| [オーナー] Character
	 *	| [　理由　] 個々のキャラクターの出すエフェクトを表現するため。
	 */

	//
	// このキャラクターの初期化を管理するコンポーネント
	//
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Components")
	TObjectPtr<UCharacterInitStateComponent> CharacterInitStateComponent;

	//
	// このキャラクターの能力を管理するコンポーネント
	//
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Components")
	TObjectPtr<UGAEAbilitySystemComponent> AbilitySystemComponent;

	//
	// このキャラクターのヘルスを管理するコンポーネント
	//
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Components")
	TObjectPtr<UHealthComponent> HealthComponent;

	//
	// このキャラクターの装備を管理するコンポーネント
	//
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Components")
	TObjectPtr<UEquipmentManagerComponent> EquipmentManagerComponent;

	//
	// このキャラクターのエフェクトを管理するコンポーネント
	//
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Components")
	TObjectPtr<UContextEffectComponent> ContextEffectComponent;

protected:
	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;

	//////////////////////////////////////////////////////////////////////////
	// Initialization
#pragma region Initialization
protected:
	UFUNCTION()
	virtual void HandleGameReady();

	UFUNCTION(BlueprintImplementableEvent, Category = "Death")
	void OnGameReady();

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Initialization")
	virtual bool IsGameReady() const;

#pragma endregion


	//////////////////////////////////////////////////////////////////////////
	// Death
#pragma region Death
protected:
	UFUNCTION()
	virtual void HandleDeathStart();

	UFUNCTION()
	virtual void HandleDeathFinish();

	UFUNCTION(BlueprintImplementableEvent, Category = "Death")
	void OnDeathStarted();

	UFUNCTION(BlueprintImplementableEvent, Category = "Death")
	void OnDeathFinished();

#pragma endregion


	//////////////////////////////////////////////////////////////////////////
	// Interface
#pragma region Interface
protected:
	virtual FGameplayTagStackContainer* GetStatTags() override { return &StatTags; }
	virtual const FGameplayTagStackContainer* GetStatTagsConst() const override { return &StatTags; }

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UTeamMemberComponent* GetTeamMemberComponent() const override;
	virtual UEquipmentManagerComponent* GetEquipmentManagerComponent() const override;
	virtual UHealthComponent* GetHealthComponent() const override;

	void PlayEffects_Implementation(
		FGameplayTag EffectTag
		, FGameplayTagContainer Contexts
		, USceneComponent* AttachToComponent	= nullptr
		, FName AttachPointName					= NAME_None
		, FVector LocationOffset				= FVector(0.0, 0.0, 0.0)
		, FRotator RotationOffset				= FRotator(0.0, 0.0, 0.0)
		, EAttachLocation::Type LocationType	= EAttachLocation::KeepRelativeOffset
		, float VolumeMultiplier				= 1.0f
		, float PitchMultiplier					= 1.0f
		, FVector VFXScale						= FVector(1.0, 1.0, 1.0)
	) override;

#pragma endregion

};
