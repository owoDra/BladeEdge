// Copyright (C) 2023 owoDra

#pragma once

#include "GAEGameplayAbility.h"

#include "BEGameplayAbility_AutoRespawn.generated.h"

class APlayerState;
class APawn;


/**
 * キャラクターの自動リスポーンのためのアビリティクラス
 */
UCLASS()
class PROJECTBE_API UBEGameplayAbility_AutoRespawn : public UGAEGameplayAbility
{
	GENERATED_BODY()
public:
	UBEGameplayAbility_AutoRespawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle
		, const FGameplayAbilityActorInfo* ActorInfo
		, const FGameplayAbilityActivationInfo ActivationInfo
		, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle
		, const FGameplayAbilityActorInfo* ActorInfo
		, const FGameplayAbilityActivationInfo ActivationInfo
		, bool bReplicateEndAbility
		, bool bWasCancelled) override;


protected:
	UPROPERTY(Transient)
	TWeakObjectPtr<APawn> CurrentPawn{ nullptr };

protected:
	void ListenPawnSet(APlayerState* InPlayerState);
	void UnlistenPawnSet(APlayerState* InPlayerState);

	UFUNCTION()
	void HandlePawnSet(APlayerState* Player, APawn* NewPawn, APawn* OldPawn);

protected:
	void ListenPawnEndPlay(APawn* InPawn);
	void UnlistenPawnEndPlay();

	UFUNCTION()
	void HandlePawnEndPlay(AActor* Actor, EEndPlayReason::Type EndPlayReason);

protected:
	virtual void Respawn();

};
