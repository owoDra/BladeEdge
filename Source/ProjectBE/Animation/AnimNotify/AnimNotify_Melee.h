// Copyright (C) 2024 owoDra

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"

#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"

#include "AnimNotify_Melee.generated.h"


/**
 * 近接攻撃の攻撃判定のタイミングを知らせる AnimNotify
 */
UCLASS(BlueprintType, meta = (DisplayName = "AN Melee"))
class UAnimNotify_Melee : public UAnimNotify
{
	GENERATED_BODY()
public:
	UAnimNotify_Melee(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(AdvancedDisplay, BlueprintReadWrite, EditAnywhere, Category = "Gameplay Event", meta = (Categories = "Event.Melee"))
	FGameplayTag EventTag;

	UPROPERTY(AdvancedDisplay, BlueprintReadWrite, EditAnywhere, Category = "Gameplay Event")
	FGameplayEventData Payload;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay Event")
	float MeleeDirection{ 0.0f };

public:
	virtual FString GetNotifyName_Implementation() const override;

	virtual void Notify(
		USkeletalMeshComponent* MeshComp, 
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

};