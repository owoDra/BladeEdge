// Copyright (C) 2024 owoDra

#pragma once

#include "Animation/AnimNotifies/AnimNotifyState.h"

#include "GameplayTagContainer.h"

#include "AnimNotifyState_Parry.generated.h"

/**
 * パリーの強度を指定する列挙
 */
UENUM(BlueprintType)
enum class EAttackParryPower : uint8
{
	Zero		= 0,

	Light		= 50,
	Normal		= 60,
	Heavy		= 70,

	Ultimate	= 255,
};


/**
 * 現在行っている攻撃のパリー可能な期間と強度を設定するAnimNotifyState
 */
UCLASS(BlueprintType, meta = (DisplayName = "ANS Parry"))
class UAnimNotifyState_Parry : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UAnimNotifyState_Parry(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parry", meta = (Categories = "Flag.Parry"))
	FGameplayTag Tag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parry")
	EAttackParryPower Power{ EAttackParryPower::Normal };

	UPROPERTY(AdvancedDisplay, BlueprintReadWrite, EditAnywhere, Category = "Parry")
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
