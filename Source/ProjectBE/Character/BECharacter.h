// Copyright (C) 2023 owoDra

#pragma once

#include "AbilityLocomotionCharacter.h"
#include "AbilitySystemInterface.h"
#include "TeamMemberComponentInterface.h"
#include "EquipmentManagerComponentInterface.h"
#include "HealthComponentInterface.h"

#include "BECharacter.generated.h"

class UGAEAbilitySystemComponent;
class UHealthComponent;
class UEquipmentManagerComponent;


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
{
	GENERATED_BODY()
public:
	explicit ABECharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

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

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UTeamMemberComponent* GetTeamMemberComponent() const override;
	virtual UEquipmentManagerComponent* GetEquipmentManagerComponent() const override;
	virtual UHealthComponent* GetHealthComponent() const override;

};
