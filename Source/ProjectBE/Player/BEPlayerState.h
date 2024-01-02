// Copyright (C) 2023 owoDra

#pragma once

#include "Actor/GFCPlayerState.h"
#include "AbilitySystemInterface.h"

#include "BEPlayerState.generated.h"

class UInitStateComponent;
class UGAEAbilitySystemComponent;


/** 
 * Base PlayerState class of this project.
 */
UCLASS(Blueprintable)
class PROJECTBE_API ABEPlayerState 
	: public AGFCPlayerState
	, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ABEPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<UInitStateComponent> InitStateComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<UGAEAbilitySystemComponent> AbilitySystemComponent;

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

};
