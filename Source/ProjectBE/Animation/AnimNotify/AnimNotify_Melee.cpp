// Copyright (C) 2024 owoDra

#include "AnimNotify_Melee.h"

#include "GameplayTag/BETags_GameplayEvent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AnimNotify_Melee)


UAnimNotify_Melee::UAnimNotify_Melee(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EventTag = TAG_Event_Melee;
}


FString UAnimNotify_Melee::GetNotifyName_Implementation() const
{
	return TEXT("Melee");
}

void UAnimNotify_Melee::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (auto* ASC{ UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MeshComp->GetOwner()) })
	{
		auto NewScopedWindow{ FScopedPredictionWindow(ASC, true) };

		ASC->HandleGameplayEvent(EventTag, &Payload);
	}
}
