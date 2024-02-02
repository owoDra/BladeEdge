// Copyright (C) 2024 owoDra

#include "AnimNotify_CameraShake.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Pawn.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AnimNotify_CameraShake)


UAnimNotify_CameraShake::UAnimNotify_CameraShake(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


FString UAnimNotify_CameraShake::GetNotifyName_Implementation() const
{
	return TEXT("Camera Shake");
}

void UAnimNotify_CameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (CameraShakeClass)
	{
		auto* Pawn{ MeshComp->GetOwner<APawn>() };
		auto* PlayerController{ Pawn ? Pawn->GetLocalViewingPlayerController() : nullptr };
		auto CameraManager{ PlayerController ? PlayerController->PlayerCameraManager : nullptr };

		if (CameraManager)
		{
			CameraManager->StartCameraShake(CameraShakeClass, Scale);
		}
	}
}
