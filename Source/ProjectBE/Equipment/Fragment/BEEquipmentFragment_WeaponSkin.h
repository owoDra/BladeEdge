// Copyright (C) 2024 owoDra

#pragma once

#include "Equipment/Fragment/EquipmentFragment.h"

#include "BEEquipmentFragment_WeaponSkin.generated.h"

class USkeletalMeshComponent;
class USkeletalMesh;
class UAnimInstance;
class APawn;


/**
 * スポーンするメッシュのデータ
 */
USTRUCT(BlueprintType)
struct FBEEquipmentMeshToSpawn
{
	GENERATED_BODY()
public:
	FBEEquipmentMeshToSpawn() = default;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<USkeletalMesh> MeshToSpawn;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UAnimInstance> MeshAnimInstance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName AttachSocket;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform AttachTransform;

public:
	bool IsValid() const;

};


/**
 * プレイヤーが設定したスキンをもとに装備のメッシュとキャラクターのアニメーションを設定するフラグメント
 */
UCLASS(meta = (DisplayName = "Weapon Skin"))
class PROJECTBE_API UBEEquipmentFragment_WeaponSkin : public UEquipmentFragment
{
	GENERATED_BODY()
public:
	UBEEquipmentFragment_WeaponSkin(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//////////////////////////////////////////////////////////////////////////////////////
	// Data Validation
public:
#if WITH_EDITOR 
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

	//////////////////////////////////////////////////////////////////////////////////////
	// Equiped / Unequiped
public:
	virtual void HandleEquiped() override;
	virtual void HandleUnequiped() override;


	//////////////////////////////////////////////////////////////////////////////////////
	// Skin Data
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Skin")
	TArray<FBEEquipmentMeshToSpawn> MeshesToSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Skin")
	TSubclassOf<UAnimInstance> FPPAnimOverlay;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Skin")
	TSubclassOf<UAnimInstance> TPPAnimOverlay;


	//////////////////////////////////////////////////////////////////////////////////////
	// Spawn Meshes
protected:
	UPROPERTY(Transient)
	TArray<TObjectPtr<USkeletalMeshComponent>> SpawnedMeshes;

protected:
	void SpawmMeshesFor(APawn* Pawn, USkeletalMeshComponent* TargetMesh);
	void RemoveSpawnedMeshes();


	//////////////////////////////////////////////////////////////////////////////////////
	// Apply Anim Overlay
protected:
	UPROPERTY(Transient)
	TWeakObjectPtr<USkeletalMeshComponent> AppliedTPPMesh{ nullptr };

	UPROPERTY(Transient)
	TWeakObjectPtr<USkeletalMeshComponent> AppliedFPPMesh{ nullptr };

protected:
	void SetAnimOverlay_TPP(USkeletalMeshComponent* TargetMesh);
	void RemoveAnimOverlay_TPP();
	void ListenMeshAnimInitialized_TPP(USkeletalMeshComponent* TargetMesh);
	void UnlistenMeshAnimInitialized_TPP(USkeletalMeshComponent* TargetMesh);

	UFUNCTION()
	void HandleMeshAnimInitialized_TPP();

	void SetAnimOverlay_FPP(USkeletalMeshComponent* TargetMesh);
	void RemoveAnimOverlay_FPP();
	void ListenMeshAnimInitialized_FPP(USkeletalMeshComponent* TargetMesh);
	void UnlistenMeshAnimInitialized_FPP(USkeletalMeshComponent* TargetMesh);

	UFUNCTION()
	void HandleMeshAnimInitialized_FPP();

};
