// Copyright (C) 2024 owoDra

#pragma once

#include "Engine/DataAsset.h"

#include "Input/GamepadSensitivityTypes.h"

#include "GamepadSensitivityData.generated.h"

/** 
 * ゲームパッドのユーザー設定の段階ごとの実際の感度を定義するデータアセット
 */
UCLASS(BlueprintType, Const)
class PROJECTBE_API UGamepadSensitivityData : public UDataAsset
{
	GENERATED_BODY()
public:
	UGamepadSensitivityData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EGamepadSensitivity, float> SensitivityMap;

public:
	const float SensitivtyEnumToFloat(const EGamepadSensitivity InSensitivity) const;
	
};