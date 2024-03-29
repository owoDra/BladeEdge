﻿// Copyright (C) 2024 owoDra

#pragma once

#include "Player/GFCPlayerState.h"
#include "AbilitySystemInterface.h"
#include "TeamMemberComponentInterface.h"
#include "GameplayTag/GameplayTagStackInterface.h"

#include "BEPlayerState.generated.h"

class UInitStateComponent;
class UGAEAbilitySystemComponent;
class UTeamMemberComponent;
class UBELoadoutComponent;


/** 
 * Base PlayerState class of this project.
 */
UCLASS(Blueprintable)
class PROJECTBE_API ABEPlayerState 
	: public AGFCPlayerState
	, public IAbilitySystemInterface
	, public ITeamMemberComponentInterface
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

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<UBELoadoutComponent> LoadoutComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	TObjectPtr<UTeamMemberComponent> TeamMemberComponent;

protected:
	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;

protected:
	virtual FGameplayTagStackContainer* GetStatTags() override { return &StatTags; }
	virtual const FGameplayTagStackContainer* GetStatTagsConst() const override { return &StatTags; }

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UTeamMemberComponent* GetTeamMemberComponent() const override;
};
