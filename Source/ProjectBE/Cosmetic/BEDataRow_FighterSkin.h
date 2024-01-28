// Copyright (C) 2024 owoDra

#pragma once

#include "Engine/DataTable.h"

#include "Recipe/CharacterSetMeshTypes.h"

#include "BEDataRow_FighterSkin.generated.h"

class UEquipment;


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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties, TitleProperty = "MeshTag"))
	TArray<FMeshToSetMesh> MeshesToSet;

};