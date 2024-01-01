// Copyright (C) 2023 owoDra

#pragma once

#include "InputModifiers.h"

#include "InputModifier_SettingScalar.generated.h"


/** 
 * 設定の値を入力値に反映させる InputModifer クラス
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "IM Setting Based Scalar"))
class UInputModifier_SettingScalar : public UInputModifier
{
	GENERATED_BODY()
public:
	UInputModifier_SettingScalar() {}

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Settings")
	FName XAxisScalarSettingName{ NAME_None };

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Settings")
	FName YAxisScalarSettingName{ NAME_None };

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Settings")
	FName ZAxisScalarSettingName{ NAME_None };
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Settings")
	FVector MaxValueClamp{ FVector(10.0, 10.0, 10.0) };
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Settings")
	FVector MinValueClamp{ FVector::ZeroVector };

private:
	TArray<const FProperty*> PropertyCache;

protected:
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;

};
