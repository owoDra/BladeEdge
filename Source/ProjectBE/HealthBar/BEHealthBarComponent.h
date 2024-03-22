// Copyright (C) 2024 owoDra

#pragma once

#include "Components/MaterialBillboardComponent.h"

#include "BEHealthBarComponent.generated.h"

class UMaterialInterface;


/**
 * アクターのヘルスを視覚的に表示するためのコンポーネント
 */
UCLASS(BlueprintType, meta = (BlueprintSpawnableComponent))
class PROJECTBE_API UBEHealthBarComponent : public UMaterialBillboardComponent
{
	GENERATED_BODY()
public:
	UBEHealthBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Material")
	FName MaxValueParamName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Material")
	FName CurrentValueParamName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Material")
	TSoftObjectPtr<UMaterialInterface> HealthMaterial;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Material")
	float SizeX{ 5.0f };

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Material")
	float SizeY{ 40.0f };

protected:
	UPROPERTY(Transient)
	FMaterialSpriteElement HealthSpriteElement;

public:
	UFUNCTION(BlueprintCallable, Category = "Health Bar")
	virtual void SetMaxHealth(float InValue);

	UFUNCTION(BlueprintCallable, Category = "Health Bar")
	virtual void SetCurrentHealth(float InValue);

protected:
	virtual void SetMaterialParam(FName Param, float Value);

};
