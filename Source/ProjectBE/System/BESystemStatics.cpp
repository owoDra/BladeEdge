// Copyright (C) 2024 owoDra

#include "BESystemStatics.h"

#include "ProjectBELogs.h"

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Engine/AssetManager.h"
#include "Engine/AssetManagerTypes.h"
#include "Components/MeshComponent.h"
#include "Components/Image.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BESystemStatics)


// AssetManager

TSoftObjectPtr<UObject> UBESystemStatics::GetTypedSoftObjectReferenceFromPrimaryAssetId(FPrimaryAssetId PrimaryAssetId, TSubclassOf<UObject> ExpectedAssetType)
{
	if (auto* Manager{ UAssetManager::GetIfInitialized() })
	{
		FPrimaryAssetTypeInfo Info;
		if (Manager->GetPrimaryAssetTypeInfo(PrimaryAssetId.PrimaryAssetType, Info) && !Info.bHasBlueprintClasses)
		{
			if (auto AssetClass{ Info.AssetBaseClassLoaded })
			{
				if ((ExpectedAssetType == nullptr) || !AssetClass->IsChildOf(ExpectedAssetType))
				{
					return nullptr;
				}
			}
			else
			{
				UE_LOG(LogBE_System, Warning, TEXT("GetTypedSoftObjectReferenceFromPrimaryAssetId(%s, %s) - AssetBaseClassLoaded was unset so we couldn't validate it, returning null"),
					*PrimaryAssetId.ToString(),
					*GetPathNameSafe(*ExpectedAssetType));
			}

			return TSoftObjectPtr<UObject>(Manager->GetPrimaryAssetPath(PrimaryAssetId));
		}
	}
	return nullptr;
}


// Project

FString UBESystemStatics::GetProjectVersion()
{
	FString AppVersion;
	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("ProjectVersion"),
		AppVersion,
		GGameIni
	);

	return AppVersion;
}


// Rendering

void UBESystemStatics::SetScalarParameterValueOnAllMeshComponents(AActor* TargetActor, const FName ParameterName, const float ParameterValue, bool bIncludeChildActors)
{
	if (TargetActor != nullptr)
	{
		TargetActor->ForEachComponent<UMeshComponent>(bIncludeChildActors, [=](UMeshComponent* InComponent)
		{
			InComponent->SetScalarParameterValueOnMaterials(ParameterName, ParameterValue);
		});
	}
}

void UBESystemStatics::SetVectorParameterValueOnAllMeshComponents(AActor* TargetActor, const FName ParameterName, const FVector ParameterValue, bool bIncludeChildActors)
{
	if (TargetActor != nullptr)
	{
		TargetActor->ForEachComponent<UMeshComponent>(bIncludeChildActors, [=](UMeshComponent* InComponent)
		{
			InComponent->SetVectorParameterValueOnMaterials(ParameterName, ParameterValue);
		});
	}
}

void UBESystemStatics::SetColorParameterValueOnAllMeshComponents(AActor* TargetActor, const FName ParameterName, const FLinearColor ParameterValue, bool bIncludeChildActors)
{
	SetVectorParameterValueOnAllMeshComponents(TargetActor, ParameterName, FVector(ParameterValue), bIncludeChildActors);
}


void UBESystemStatics::SetScalarParameterValueOnImage(UImage* Image, const FName ParameterName, const float ParameterValue)
{
	if (auto* DynamicMaterial{ Image ? Image->GetDynamicMaterial() : nullptr })
	{
		DynamicMaterial->SetScalarParameterValue(ParameterName, ParameterValue);
	}
}

void UBESystemStatics::SetScalarParameterValueOnImages(const TArray<UImage*>& Images, const FName ParameterName, const float ParameterValue)
{
	for (const auto& Image : Images)
	{
		if (auto* DynamicMaterial{ Image ? Image->GetDynamicMaterial() : nullptr })
		{
			DynamicMaterial->SetScalarParameterValue(ParameterName, ParameterValue);
		}
	}
}

void UBESystemStatics::SetScalarParametersValueOnImage(UImage* Image, const TMap<FName, float>& Parameters)
{
	if (auto* DynamicMaterial{ Image ? Image->GetDynamicMaterial() : nullptr })
	{
		for (const auto& KVP : Parameters)
		{
			DynamicMaterial->SetScalarParameterValue(KVP.Key, KVP.Value);
		}
	}
}

void UBESystemStatics::SetScalarParametersValueOnImages(const TArray<UImage*>& Images, const TMap<FName, float>& Parameters)
{
	for (const auto& Image : Images)
	{
		if (auto* DynamicMaterial{ Image ? Image->GetDynamicMaterial() : nullptr })
		{
			for (const auto& KVP : Parameters)
			{
				DynamicMaterial->SetScalarParameterValue(KVP.Key, KVP.Value);
			}
		}
	}
}


// Components

TArray<UActorComponent*> UBESystemStatics::FindComponentsByClass(AActor* TargetActor, TSubclassOf<UActorComponent> ComponentClass, bool bIncludeChildActors)
{
	TArray<UActorComponent*> Components;
	if (TargetActor != nullptr)
	{
		TargetActor->GetComponents(ComponentClass, /*out*/ Components, bIncludeChildActors);

	}
	return MoveTemp(Components);
}
