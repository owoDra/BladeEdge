// Copyright (C) 2024 owoDra

#pragma once

#include "Engine/DataTable.h"

#include "BEDataRow_WeaponSkin.generated.h"

class UEquipment;


/**
 * 武器スキンごとの Equipment クラスを登録するためのデータテーブルに使用するデータ
 */
USTRUCT(BlueprintType)
struct FBEDataRow_WeaponSkin : public FTableRowBase
{
	GENERATED_BODY()
public:
	FBEDataRow_WeaponSkin() {}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UEquipment> EquipmentClass;

};