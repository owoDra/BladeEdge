// Copyright (C) 2024 owoDra

#pragma once

#include "GameMode/GFCGameState.h"
#include "AbilitySystemInterface.h"

#include "BEGameState.generated.h"

class UExperienceDataComponent;
class UGAEAbilitySystemComponent;
class UGamePhaseComponent;
class ULoadingProcessTask;


/** 
 * Base GameState class of this project.
 */
UCLASS(Blueprintable)
class PROJECTBE_API ABEGameState 
	: public AGFCGameStateBase
	, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ABEGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static const FName NAME_GameStateLoading;

	UPROPERTY()
	FText GameStateLoadingReason;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<UExperienceDataComponent> ExperienceDataComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<UGAEAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<UGamePhaseComponent> GamePhaseComponent;

public:
	virtual void BeginPlay() override;

protected:
	void HandleGameReady();


public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

};
