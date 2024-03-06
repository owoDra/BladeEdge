// Copyright (C) 2024 owoDra

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "BESystemStatics.generated.h"

class AActor;
class UImage;


UCLASS()
class UBESystemStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	//////////////////////////////////////////////////////
	// AssetManager
public:
	UFUNCTION(BlueprintPure, Category = "AssetManager", meta=(DeterminesOutputType=ExpectedAssetType))
	static TSoftObjectPtr<UObject> GetTypedSoftObjectReferenceFromPrimaryAssetId(FPrimaryAssetId PrimaryAssetId, TSubclassOf<UObject> ExpectedAssetType);


	//////////////////////////////////////////////////////
	// Project
public:
	UFUNCTION(BlueprintCallable, Category = "Project")
	static FString GetProjectVersion();


	//////////////////////////////////////////////////////
	// Rendering
public:
	UFUNCTION(BlueprintCallable, Category = "Rendering|Material", meta=(DefaultToSelf="TargetActor"))
	static void SetScalarParameterValueOnAllMeshComponents(AActor* TargetActor, const FName ParameterName, const float ParameterValue, bool bIncludeChildActors = true);

	UFUNCTION(BlueprintCallable, Category = "Rendering|Material", meta=(DefaultToSelf="TargetActor"))
	static void SetVectorParameterValueOnAllMeshComponents(AActor* TargetActor, const FName ParameterName, const FVector ParameterValue, bool bIncludeChildActors = true);

	UFUNCTION(BlueprintCallable, Category = "Rendering|Material", meta=(DefaultToSelf="TargetActor"))
	static void SetColorParameterValueOnAllMeshComponents(AActor* TargetActor, const FName ParameterName, const FLinearColor ParameterValue, bool bIncludeChildActors = true);

public:
	UFUNCTION(BlueprintCallable, Category = "Rendering|Material")
	static void SetScalarParameterValueOnImage(UImage* Image, const FName ParameterName, const float ParameterValue);

	UFUNCTION(BlueprintCallable, Category = "Rendering|Material")
	static void SetScalarParameterValueOnImages(const TArray<UImage*>& Images, const FName ParameterName, const float ParameterValue);

	UFUNCTION(BlueprintCallable, Category = "Rendering|Material")
	static void SetScalarParametersValueOnImage(UImage* Image, const TMap<FName, float>& Parameters);

	UFUNCTION(BlueprintCallable, Category = "Rendering|Material")
	static void SetScalarParametersValueOnImages(const TArray<UImage*>& Images, const TMap<FName, float>& Parameters);


	//////////////////////////////////////////////////////
	// Components
public:
	UFUNCTION(BlueprintCallable, Category = "Components", meta=(DefaultToSelf="TargetActor", ComponentClass="/Script/Engine.ActorComponent", DeterminesOutputType="ComponentClass"))
	static TArray<UActorComponent*> FindComponentsByClass(AActor* TargetActor, TSubclassOf<UActorComponent> ComponentClass, bool bIncludeChildActors = true);

};
