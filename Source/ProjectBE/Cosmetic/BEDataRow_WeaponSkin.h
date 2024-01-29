// Copyright (C) 2024 owoDra

#pragma once

#include "Engine/DataTable.h"

#include "GameplayTagContainer.h"

#include "BEDataRow_WeaponSkin.generated.h"

class UAnimInstance;
class USkeletalMesh;


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
	TSoftObjectPtr<USkeletalMesh> MeshToSpawn;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftClassPtr<UAnimInstance> MeshAnimInstance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName AttachSocket;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform AttachTransform;

public:
	bool IsValid() const;

};


/**
 * 武器スキンごとスポーンするメッシュとキャラクターに適用するアニメーションのデータ
 */
USTRUCT(BlueprintType)
struct FBEDataRow_WeaponSkin : public FTableRowBase
{
	GENERATED_BODY()
public:
	FBEDataRow_WeaponSkin() {}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FBEEquipmentMeshToSpawn> MeshesToSpawn;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftClassPtr<UAnimInstance> FPPAnimOverlay;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftClassPtr<UAnimInstance> TPPAnimOverlay;

public:
	bool IsValid() const;

};
