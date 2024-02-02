// Copyright (C) 2024 owoDra

#pragma once

#include "Animation/AnimNotifies/AnimNotifyState.h"

#include "GameplayTagContainer.h"

#include "AnimNotifyState_DynamicPlayRate.generated.h"


/**
 * 動的にモンタージュの再生速度を変更する AnimNotifyState
 */
UCLASS(BlueprintType, meta = (DisplayName = "ANS Dynamic Play Rate"))
class UAnimNotifyState_DynamicPlayRate : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UAnimNotifyState_DynamicPlayRate(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(AdvancedDisplay, BlueprintReadWrite, EditAnywhere, Category = "AnimCurveName")
	FName DynamicPlayRateCurveName{ TEXTVIEW("DynamicPlayRate") };

public:
	virtual FString GetNotifyName_Implementation() const override;

	virtual void NotifyTick(
		USkeletalMeshComponent* MeshComp
		, UAnimSequenceBase* Animation
		, float FrameDeltaTime
		, const FAnimNotifyEventReference& EventReference) override;

};