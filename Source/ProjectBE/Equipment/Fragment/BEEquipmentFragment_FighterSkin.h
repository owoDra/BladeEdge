// Copyright (C) 2024 owoDra

#pragma once

#include "Equipment/Fragment/EquipmentFragment.h"

#include "GameplayTagContainer.h"

#include "BEEquipmentFragment_FighterSkin.generated.h"

class USkeletalMeshComponent;
class UContextEffectLibrary;
class USkeletalMesh;
class UAnimInstance;


/**
 * 設定するメッシュのデータ
 */
USTRUCT(BlueprintType)
struct FBEMeshToSetMesh
{
	GENERATED_BODY()
public:
	FBEMeshToSetMesh() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<USkeletalMesh> SkeletalMesh{ nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UAnimInstance> AnimInstance{ nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector NewLocation{ 0.0f, 0.0f, -90.0f };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator NewRotation{ 0.0f, -90.0f, 0.0f };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector NewScale{ FVector::OneVector };

public:
	bool IsValid() const;

};


/**
 * プレイヤーが設定したスキンをもとにキャラクターのアニメーションやエフェクトを設定するフラグメント
 */
UCLASS(meta = (DisplayName = "Fighter Skin"))
class PROJECTBE_API UBEEquipmentFragment_FighterSkin : public UEquipmentFragment
{
	GENERATED_BODY()
public:
	UBEEquipmentFragment_FighterSkin(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//////////////////////////////////////////////////////////////////////////////////////
	// Data Validation
public:
#if WITH_EDITOR 
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif


	//////////////////////////////////////////////////////////////////////////////////////
	// Equiped / Unequiped
public:
	virtual void HandleEquipmentGiven() override;


	//////////////////////////////////////////////////////////////////////////////////////
	// Skin Data
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Fighter Skin")
	FBEMeshToSetMesh TPPMeshesToSet;

	UPROPERTY(EditDefaultsOnly, Category = "Fighter Skin")
	FBEMeshToSetMesh FPPMeshesToSet;

	UPROPERTY(EditDefaultsOnly, Category = "Fighter Skin")
	TSet<TSoftObjectPtr<UContextEffectLibrary>> ContextEffectLibraries;

	UPROPERTY(EditDefaultsOnly, Category = "Fighter Skin")
	FGameplayTagContainer ExtraContexts;


	//////////////////////////////////////////////////////////////////////////////////////
	// Set Meshes
protected:
	void SetMesh(USkeletalMeshComponent* TargetMesh, const FBEMeshToSetMesh& Info);


	//////////////////////////////////////////////////////////////////////////////////////
	// Set Context Effect
protected:
	void SetContextEffectLibraries(APawn* Pawn);

};
