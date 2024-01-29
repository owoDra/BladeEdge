// Copyright (C) 2024 owoDra

#pragma once

#include "Equipment/Fragment/EquipmentFragment.h"

#include "Cosmetic/BEDataRow_WeaponSkin.h"

#include "BEEquipmentFragment_WeaponSkin.generated.h"

class USkeletalMeshComponent;
class UDataTable;
class APawn;


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
	//
	// スキンのデータテーブル
	//
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Skin", meta = (RowType = "BEDataRow_WeaponSkin"))
	TObjectPtr<const UDataTable> DataTable{ nullptr };

	//
	// 現在適用中のスキンのデータ
	//
	UPROPERTY(Transient);
	FBEDataRow_WeaponSkin SkinData;

protected:
	void StoreSkinData(APawn* Pawn);


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
	struct FApplyingOverlay
	{
		TWeakObjectPtr<USkeletalMeshComponent> TargetMesh{ nullptr };
		TSubclassOf<UAnimInstance> ApplingOverlayClass{ nullptr };
	};

	FApplyingOverlay ApplingOverlay_TPP;
	FApplyingOverlay ApplingOverlay_FPP;

protected:
	void SetAnimOverlay_TPP(USkeletalMeshComponent* TargetMesh);
	void RemoveAnimOverlay_TPP();
	void ListenMeshAnimInitialized_TPP(USkeletalMeshComponent* TargetMesh);
	void UnlistenMeshAnimInitialized_TPP(USkeletalMeshComponent* TargetMesh);
	void HandleMeshAnimInitialized_TPP();

	void SetAnimOverlay_FPP(USkeletalMeshComponent* TargetMesh);
	void RemoveAnimOverlay_FPP();
	void ListenMeshAnimInitialized_FPP(USkeletalMeshComponent* TargetMesh);
	void UnlistenMeshAnimInitialized_FPP(USkeletalMeshComponent* TargetMesh);
	void HandleMeshAnimInitialized_FPP();

};
