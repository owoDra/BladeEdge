// Copyright (C) 2023 owoDra

#pragma once

#include "Actor/GFCGameState.h"
#include "AbilitySystemInterface.h"

#include "BEGameState.generated.h"

class UExperienceDataComponent;
class UGAEAbilitySystemComponent;


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

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<UExperienceDataComponent> ExperienceDataComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<UGAEAbilitySystemComponent> AbilitySystemComponent;

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

};
