// Copyright (C) 2024 owoDra

#include "BEHealthBarComponent.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEHealthBarComponent)


UBEHealthBarComponent::UBEHealthBarComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetOwnerNoSee(true);
}


void UBEHealthBarComponent::SetMaxHealth(float InValue)
{
	SetMaterialParam(MaxValueParamName, InValue);
}

void UBEHealthBarComponent::SetCurrentHealth(float InValue)
{
	SetMaterialParam(CurrentValueParamName, InValue);
}

void UBEHealthBarComponent::SetMaterialParam(FName Param, float Value)
{
	// Dynamic Instance が作成されていない場合はこのタイミングで作成

	if (!HealthSpriteElement.Material)
	{
		auto* Material{ HealthMaterial.IsValid() ? HealthMaterial.Get() : HealthMaterial.LoadSynchronous() };
		auto* DynamicMaterial{ UMaterialInstanceDynamic::Create(Material, this) };

		HealthSpriteElement.Material = DynamicMaterial;
	}

	// パラメーターを更新

	auto* DynamicMaterial{ Cast<UMaterialInstanceDynamic>(HealthSpriteElement.Material) };
	if (ensure(DynamicMaterial))
	{
		DynamicMaterial->SetScalarParameterValue(Param, Value);
	}

	HealthSpriteElement.BaseSizeX = SizeX;
	HealthSpriteElement.BaseSizeY = SizeY;

	// エレメントを置き換え

	TArray<FMaterialSpriteElement> NewElements{ HealthSpriteElement };
	SetElements(NewElements);
}
