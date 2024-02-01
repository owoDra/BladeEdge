// Copyright (C) 2024 owoDra

#include "AnimNotifyState_ComboReady.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AnimNotifyState_ComboReady)


UAnimNotifyState_ComboReady::UAnimNotifyState_ComboReady(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


FString UAnimNotifyState_ComboReady::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("Combo Ready: %s"), *Tag.GetTagName().ToString());
}

void UAnimNotifyState_ComboReady::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (auto* ASC{ UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MeshComp->GetOwner()) })
	{
		ASC->AddLooseGameplayTag(Tag);

		if (bShouldReplicate)
		{
			ASC->AddReplicatedLooseGameplayTag(Tag);
		}
	}
}

void UAnimNotifyState_ComboReady::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (auto* ASC{ UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MeshComp->GetOwner()) })
	{
		ASC->RemoveLooseGameplayTag(Tag);

		if (bShouldReplicate)
		{
			ASC->RemoveReplicatedLooseGameplayTag(Tag);
		}
	}
}
