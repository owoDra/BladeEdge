// Copyright (C) 2024 owoDra

#include "BEEquipmentFragment_FighterSpec.h"

// Game Ability: Health Addon
#include "HealthFunctionLibrary.h"
#include "HealthComponent.h"
#include "HealthData.h"

// Game Locomotion Extension
#include "LocomotionComponent.h"
#include "LocomotionData.h"

// Engine Feature
#include "GameFramework/Character.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEEquipmentFragment_FighterSpec)

////////////////////////////////////////////////////////////////////////////

UBEEquipmentFragment_FighterSpec::UBEEquipmentFragment_FighterSpec(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NetExecutionPolicy = EEquipmentFragmentNetExecutionPolicy::ServerOnly;

#if WITH_EDITOR
	StaticClass()->FindPropertyByName(FName{ TEXTVIEW("NetExecutionPolicy") })->SetPropertyFlags(CPF_DisableEditOnTemplate);
#endif
}

#if WITH_EDITOR 
EDataValidationResult UBEEquipmentFragment_FighterSpec::IsDataValid(FDataValidationContext& Context) const
{
	auto Result{ CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid) };

	if (!HealthData)
	{
		Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);

		Context.AddError(FText::FromString(FString::Printf(TEXT("Invalid HealthData is set in %s"), *GetNameSafe(this))));
	}

	if (!LocomotionData)
	{
		Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);

		Context.AddError(FText::FromString(FString::Printf(TEXT("Invalid LocomotionData is set in %s"), *GetNameSafe(this))));
	}

	return Result;
}
#endif


// Given

void UBEEquipmentFragment_FighterSpec::HandleEquipmentGiven()
{
	Super::HandleEquipmentGiven();

	if (auto* Character{ GetOwner<ACharacter>() })
	{
		if (auto* HC{ UHealthFunctionLibrary::GetHealthComponentFromActor(Character) })
		{
			HC->SetHealthData(HealthData);
		}

		if (auto* LC{ ULocomotionComponent::FindLocomotionComponent(Character) })
		{
			LC->SetLocomotionData(LocomotionData);
		}
	}
}
