// Copyright (C) 2024 owoDra

#pragma once

#include "Equipment/Fragment/EquipmentFragment.h"

#include "GameplayTagContainer.h"

#include "BEEquipmentFragment_FighterSpec.generated.h"

class UHealthData;
class ULocomotionData;


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


	//////////////////////////////////////////////////////////////////////////////////////
	// Skin Data
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Fighter Spec")
	TObjectPtr<UHealthData> HealthData{ nullptr };

	UPROPERTY(EditDefaultsOnly, Category = "Fighter Spec")
	TObjectPtr<ULocomotionData> LocomotionData{ nullptr };

};
