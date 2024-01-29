// Copyright (C) 2024 owoDra

#pragma once

#include "Engine/DataTable.h"

#include "GameplayTagContainer.h"

#include "BEDataRow_FighterSkin.generated.h"

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
	TSoftObjectPtr<USkeletalMesh> SkeletalMesh{ nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftClassPtr<UAnimInstance> AnimInstance{ nullptr };

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
 * ファイタースキンごとのメッシュとアニメーションを登録するためのデータテーブルに使用するデータ
 */
USTRUCT(BlueprintType)
struct FBEDataRow_FighterSkin : public FTableRowBase
{
	GENERATED_BODY()
public:
	FBEDataRow_FighterSkin() {}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FBEMeshToSetMesh TPPMeshesToSet;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FBEMeshToSetMesh FPPMeshesToSet;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSet<TSoftObjectPtr<UContextEffectLibrary>> ContextEffectLibraries;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTagContainer ExtraContexts;

public:
	bool IsValid() const;

};