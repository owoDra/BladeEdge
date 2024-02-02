// Copyright (C) 2024 owoDra

#include "AnimNotifyState_DynamicPlayRate.h"

#include "Components/SkeletalMeshComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AnimNotifyState_DynamicPlayRate)


UAnimNotifyState_DynamicPlayRate::UAnimNotifyState_DynamicPlayRate(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


FString UAnimNotifyState_DynamicPlayRate::GetNotifyName_Implementation() const
{
	return TEXT("Dynamic Play Rate");
}

void UAnimNotifyState_DynamicPlayRate::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	auto* AnimInstance{ MeshComp->GetAnimInstance() };
	auto* AnimMontage{ Cast<UAnimMontage>(Animation) };

	if (AnimInstance && AnimMontage)
	{
		auto PlayRate{ 1.0f };
		AnimInstance->GetCurveValueWithDefault(DynamicPlayRateCurveName, 1.0f, PlayRate);
		AnimInstance->Montage_SetPlayRate(AnimMontage, PlayRate);
	}
}
