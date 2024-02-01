// Copyright (C) 2024 owoDra

#include "AnimNotifyState_ComboStandby.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AnimNotifyState_ComboStandby)


UAnimNotifyState_ComboStandby::UAnimNotifyState_ComboStandby(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


FString UAnimNotifyState_ComboStandby::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("Combo Standby: %s"), *Tag.GetTagName().ToString());
}

void UAnimNotifyState_ComboStandby::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (auto* ASC{ UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MeshComp->GetOwner()) })
	{
		ASC->SetLooseGameplayTagCount(Tag, 1);

		if (bShouldReplicate)
		{
			ASC->SetReplicatedLooseGameplayTagCount(Tag, 1);
		}
	}
}

void UAnimNotifyState_ComboStandby::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (auto* ASC{ UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MeshComp->GetOwner()) })
	{
		ASC->SetLooseGameplayTagCount(Tag, 0);

		if (bShouldReplicate)
		{
			ASC->SetReplicatedLooseGameplayTagCount(Tag, 0);
		}
	}
}
