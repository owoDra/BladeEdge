// Copyright (C) 2024 owoDra

#include "BEEquipmentFragment_FighterSpec.h"

#include "GameplayTag/BETags_Input.h"

// Game Ability: Health Addon
#include "HealthFunctionLibrary.h"
#include "HealthComponent.h"
#include "HealthData.h"

// Game Locomotion Extension
#include "LocomotionComponent.h"
#include "LocomotionData.h"

// Engine Feature
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Character.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEEquipmentFragment_FighterSpec)

////////////////////////////////////////////////////////////////////////////

UBEEquipmentFragment_FighterSpec::UBEEquipmentFragment_FighterSpec(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NetExecutionPolicy = EEquipmentFragmentNetExecutionPolicy::Both;

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

	if (!JumpAbility)
	{
		Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);

		Context.AddError(FText::FromString(FString::Printf(TEXT("Invalid JumpAbility is set in %s"), *GetNameSafe(this))));
	}

	if (!DodgeAbility)
	{
		Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);

		Context.AddError(FText::FromString(FString::Printf(TEXT("Invalid DodgeAbility is set in %s"), *GetNameSafe(this))));
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
		ApplyHealthData(Character);
		ApplyLocomotionData(Character);
		ApplyAbilities(Character);
	}
}

void UBEEquipmentFragment_FighterSpec::HandleEquipmentRemove()
{
	Super::HandleEquipmentRemove();

	if (auto* Character{ GetOwner<ACharacter>() })
	{
		RemoveAbilities(Character);
	}
}


void UBEEquipmentFragment_FighterSpec::ApplyHealthData(ACharacter* Character)
{
	if (auto* HC{ UHealthFunctionLibrary::GetHealthComponentFromActor(Character) })
	{
		HC->SetHealthData(HealthData);
	}
}

void UBEEquipmentFragment_FighterSpec::ApplyLocomotionData(ACharacter* Character)
{
	if (auto* LC{ ULocomotionComponent::FindLocomotionComponent(Character) })
	{
		LC->SetLocomotionData(LocomotionData);
	}
}

void UBEEquipmentFragment_FighterSpec::ApplyAbilities(ACharacter* Character)
{
	if (Character->HasAuthority())
	{
		if (auto* ASC{ UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Character) })
		{
			{
				auto* AbilityCDO{ JumpAbility->GetDefaultObject<UGameplayAbility>() };
				auto AbilitySpec{ FGameplayAbilitySpec(AbilityCDO, 0) };
				AbilitySpec.SourceObject = Character;
				AbilitySpec.DynamicAbilityTags.AddTag(TAG_Input_Shared_Jump);

				JumpAbilitySpecHandle = ASC->GiveAbility(AbilitySpec);
			}

			{
				auto* AbilityCDO{ DodgeAbility->GetDefaultObject<UGameplayAbility>() };
				auto AbilitySpec{ FGameplayAbilitySpec(AbilityCDO, 0) };
				AbilitySpec.SourceObject = Character;
				AbilitySpec.DynamicAbilityTags.AddTag(TAG_Input_Shared_Dodge);

				DodgeAbilitySpecHandle = ASC->GiveAbility(AbilitySpec);
			}
		}
	}
}


void UBEEquipmentFragment_FighterSpec::RemoveAbilities(ACharacter* Character)
{
	if (Character->HasAuthority())
	{
		if (auto* ASC{ UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Character) })
		{
			if (JumpAbilitySpecHandle.IsValid())
			{
				ASC->ClearAbility(JumpAbilitySpecHandle);
			}

			if (DodgeAbilitySpecHandle.IsValid())
			{
				ASC->ClearAbility(DodgeAbilitySpecHandle);
			}
		}
	}
}
