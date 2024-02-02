// Copyright (C) 2024 owoDra

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"

#include "AnimNotify_CameraShake.generated.h"

class UCameraShakeBase;


/**
 * カメラシェイクを知らせる AnimNotify
 */
UCLASS(BlueprintType, meta = (DisplayName = "AN Camera Shake"))
class UAnimNotify_CameraShake : public UAnimNotify
{
	GENERATED_BODY()
public:
	UAnimNotify_CameraShake(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera Shake")
	TSubclassOf<UCameraShakeBase> CameraShakeClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera Shake")
	float Scale{ 1.0f };

public:
	virtual FString GetNotifyName_Implementation() const override;

	virtual void Notify(
		USkeletalMeshComponent* MeshComp, 
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

};