// Copyright (C) 2024 owoDra

#include "AnimNotifyState_Parry.h"

#include "GameplayTag/BETags_Flag.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AnimNotifyState_Parry)


UAnimNotifyState_Parry::UAnimNotifyState_Parry(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Tag = TAG_Flag_Parry;
}


FString UAnimNotifyState_Parry::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("Parry: %d"), static_cast<int32>(Power));
}

void UAnimNotifyState_Parry::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (auto* ASC{ UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MeshComp->GetOwner()) })
	{
		ASC->SetLooseGameplayTagCount(Tag, static_cast<int32>(Power));

		if (bShouldReplicate)
		{
			ASC->SetReplicatedLooseGameplayTagCount(Tag, static_cast<int32>(Power));
		}
	}
}

void UAnimNotifyState_Parry::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
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
