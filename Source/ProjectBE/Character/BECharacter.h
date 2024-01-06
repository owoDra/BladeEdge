// Copyright (C) 2023 owoDra

#pragma once

#include "AbilityLocomotionCharacter.h"
#include "AbilitySystemInterface.h"
#include "TeamMemberComponentInterface.h"
#include "EquipmentManagerComponentInterface.h"
#include "HealthComponentInterface.h"
#include "ContextEffectInterface.h"

#include "BECharacter.generated.h"

class UGAEAbilitySystemComponent;
class UHealthComponent;
class UEquipmentManagerComponent;
class UContextEffectComponent;
class ULoadingProcessTask;


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
{
	GENERATED_BODY()
public:
	explicit ABECharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static const FString CharacterLoadingReason;

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
	// このキャラクターの能力を管理するコンポーネント
	//
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<UGAEAbilitySystemComponent> AbilitySystemComponent;

	//
	// このキャラクターのヘルスを管理するコンポーネント
	//
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<UHealthComponent> HealthComponent;

	//
	// このキャラクターの装備を管理するコンポーネント
	//
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<UEquipmentManagerComponent> EquipmentManagerComponent;

	//
	// このキャラクターのエフェクトを管理するコンポーネント
	//
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<UContextEffectComponent> ContextEffectComponent;

	UPROPERTY(Transient)
	TObjectPtr<ULoadingProcessTask> LoadingProcessTask{ nullptr };

public:
	virtual void BeginPlay() override;

protected:
	void HandleGameReady();


public:
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
};
