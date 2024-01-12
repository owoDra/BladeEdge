// Copyright (C) 2024 owoDra

#pragma once

#include "Actor/GFCPlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayTag/GameplayTagStackInterface.h"

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
	, public IGameplayTagStackInterface
{
	GENERATED_BODY()
public:
	ABEPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<UInitStateComponent> InitStateComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<UGAEAbilitySystemComponent> AbilitySystemComponent;

protected:
	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;

protected:
	virtual FGameplayTagStackContainer* GetStatTags() override { return &StatTags; }
	virtual const FGameplayTagStackContainer* GetStatTagsConst() const override { return &StatTags; }

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

};
