// Copyright (C) 2024 owoDra

#pragma once

#include "Equipment/Fragment/EquipmentFragment.h"

#include "GameplayTagContainer.h"
#include "GameplayAbilitySpec.h"

#include "BEEquipmentFragment_FighterSpec.generated.h"

class UHealthData;
class ULocomotionData;
class ACharacter;


/**
 * ファイターの基本性能にあたるデータの設定を行うフラグメント
 */
UCLASS(meta = (DisplayName = "Fighter Spec"))
class PROJECTBE_API UBEEquipmentFragment_FighterSpec : public UEquipmentFragment
{
	GENERATED_BODY()
public:
	UBEEquipmentFragment_FighterSpec(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//////////////////////////////////////////////////////////////////////////////////////
	// Data Validation
public:
#if WITH_EDITOR 
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif


	//////////////////////////////////////////////////////////////////////////////////////
	// Given
public:
	virtual void HandleEquipmentGiven() override;
	virtual void HandleEquipmentRemove() override;


	//////////////////////////////////////////////////////////////////////////////////////
	// Skin Data
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Fighter Spec")
	TObjectPtr<UHealthData> HealthData{ nullptr };

	UPROPERTY(EditDefaultsOnly, Category = "Fighter Spec")
	TObjectPtr<ULocomotionData> LocomotionData{ nullptr };

	UPROPERTY(EditDefaultsOnly, Category = "Fighter Spec")
	TSubclassOf<UGameplayAbility> JumpAbility{ nullptr };

	UPROPERTY(EditDefaultsOnly, Category = "Fighter Spec")
	TSubclassOf<UGameplayAbility> DodgeAbility{ nullptr };

protected:
	UPROPERTY(Transient)
	FGameplayAbilitySpecHandle JumpAbilitySpecHandle;

	UPROPERTY(Transient)
	FGameplayAbilitySpecHandle DodgeAbilitySpecHandle;

protected:
	virtual void ApplyHealthData(ACharacter* Character);
	virtual void ApplyLocomotionData(ACharacter* Character);
	virtual void ApplyAbilities(ACharacter* Character);

	virtual void RemoveAbilities(ACharacter* Character);

};
