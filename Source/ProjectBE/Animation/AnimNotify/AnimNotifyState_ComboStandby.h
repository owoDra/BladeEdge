// Copyright (C) 2024 owoDra

#pragma once

#include "Animation/AnimNotifies/AnimNotifyState.h"

#include "GameplayTagContainer.h"

#include "AnimNotifyState_ComboStandby.generated.h"


/**
 * コンボの準備が完了したことを知らせる AnimNotifyState
 */
UCLASS(BlueprintType, meta = (DisplayName = "ANS Combo Standby"))
class UAnimNotifyState_ComboStandby : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UAnimNotifyState_ComboStandby(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ComboStandby", meta = (Categories = "Combo.Stanby"))
	FGameplayTag Tag;

	UPROPERTY(AdvancedDisplay, BlueprintReadWrite, EditAnywhere, Category = "ComboStandby")
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