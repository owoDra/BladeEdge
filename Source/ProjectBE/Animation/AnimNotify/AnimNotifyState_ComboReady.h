// Copyright (C) 2024 owoDra

#pragma once

#include "Animation/AnimNotifies/AnimNotifyState.h"

#include "GameplayTagContainer.h"

#include "AnimNotifyState_ComboReady.generated.h"


/**
 * コンボの実行可能を知らせる AnimNotifyState
 */
UCLASS(BlueprintType, meta = (DisplayName = "ANS Combo Ready"))
class UAnimNotifyState_ComboReady : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UAnimNotifyState_ComboReady(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ComboReady", meta = (Categories = "Combo.Ready"))
	FGameplayTag Tag;

	UPROPERTY(AdvancedDisplay, BlueprintReadWrite, EditAnywhere, Category = "ComboReady")
	bool bShouldReplicate{ false };

public:
	virtual FString GetNotifyName_Implementation() const override;

	virtual void NotifyBegin(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		float TotalDuration, 
		const FAnimNotifyEventReference& EventReference) override;
	
	virtual void NotifyEnd(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

};