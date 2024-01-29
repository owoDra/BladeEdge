// Copyright (C) 2024 owoDra

#pragma once

#include "Equipment/Fragment/EquipmentFragment.h"

#include "Cosmetic/BEDataRow_FighterSkin.h"

#include "BEEquipmentFragment_FighterSkin.generated.h"

class USkeletalMeshComponent;


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
	virtual void HandleEquiped() override;


	//////////////////////////////////////////////////////////////////////////////////////
	// Skin Data
protected:
	//
	// スキンのデータテーブル
	//
	UPROPERTY(EditDefaultsOnly, Category = "Fighter Skin", meta = (RowType = "BEDataRow_FighterSkin"))
	TObjectPtr<const UDataTable> DataTable{ nullptr };

	//
	// 現在適用中のスキンのデータ
	//
	UPROPERTY(Transient);
	FBEDataRow_FighterSkin SkinData;

protected:
	void StoreSkinData(APawn* Pawn);


	//////////////////////////////////////////////////////////////////////////////////////
	// Set Meshes
protected:
	void SetMesh(USkeletalMeshComponent* TargetMesh, const FBEMeshToSetMesh& Info);


	//////////////////////////////////////////////////////////////////////////////////////
	// Set Context Effect
protected:
	void SetContextEffectLibraries(APawn* Pawn);

};
