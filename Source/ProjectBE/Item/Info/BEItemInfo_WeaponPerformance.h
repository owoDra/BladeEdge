// Copyright (C) 2024 owoDra

#pragma once

#include "Info/ItemInfo.h"

#include "BEItemInfo_WeaponPerformance.generated.h"


/**
 * 武器のおおよその性能を表す指標
 */
UENUM(BlueprintType)
enum class EWeaponPerformance : uint8
{
	D, C, B, A, S
};


/**
 * 武器の大体の性能を表すための情報
 */
UCLASS(meta = (DisplayName = "Weapon Performance Info"))
class PROJECTBE_API UBEItemInfo_WeaponPerformance : public UItemInfo
{
	GENERATED_BODY()
public:
	UBEItemInfo_WeaponPerformance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Performance")
	EWeaponPerformance Strength;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Performance")
	EWeaponPerformance Agility;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Performance")
	EWeaponPerformance Avoidance;
};
