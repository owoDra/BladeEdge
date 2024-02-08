// Copyright (C) 2024 owoDra

#pragma once

#include "Resolver/SettingUITypeResolver.h"

#include "UserSettings/EnhancedInputUserSettings.h"

#include "BESettingUITypeResolver_Keybind.generated.h"


/**
 * キーバインド用のリゾルバークラス
 */
UCLASS(Blueprintable, meta = (DisplayName = "Keybind Resolver"))
class UBESettingUITypeResolver_Keybind final : public USettingUITypeResolver
{
	GENERATED_BODY()
public:
	UBESettingUITypeResolver_Keybind(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	////////////////////////////////////////////////////////////////////////////
	// Initialization
protected:
	UPROPERTY(Transient)
	TWeakObjectPtr<UEnhancedInputUserSettings> InputSetting;

public:
	virtual void SetInputSetting(UEnhancedInputUserSettings* InSetting) { check(InSetting); InputSetting = InSetting; }


	////////////////////////////////////////////////////////////////////////////
	// Getter・Setter
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
	EPlayerMappableKeySlot PrimarySlot{ EPlayerMappableKeySlot::First };

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
	EPlayerMappableKeySlot SecondarySlot{ EPlayerMappableKeySlot::Second };


public:
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Setting")
	virtual void SetPrimary(FKey NewKey);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Setting")
	virtual void SetSecondary(FKey NewKey);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Setting")
	virtual void ResetBinds();

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Setting")
	virtual void GetBinds(FKey& OutPrimary, FKey& OutSecondary) const;

};
