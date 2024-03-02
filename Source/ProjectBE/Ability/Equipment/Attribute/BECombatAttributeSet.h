// Copyright (C) 2024 owoDra

#pragma once

#include "GAEAttributeSet.h"

#include "AbilitySystemComponent.h"

#include "BECombatAttributeSet.generated.h"

struct FGameplayEffectModCallbackData;


/**
 * 戦闘時の能力を表すアトリビュートを定義する
 */
UCLASS(BlueprintType)
class UBECombatAttributeSet : public UGAEAttributeSet
{
	GENERATED_BODY()
public:
	UBECombatAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	ATTRIBUTE_ACCESSORS(UBECombatAttributeSet, AttackSpeed);

protected:
	UFUNCTION() void OnRep_AttackSpeed(const FGameplayAttributeData& OldValue);

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackSpeed, Category = "Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackSpeed;

};
