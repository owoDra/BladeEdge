// Copyright (C) 2024 owoDra

#pragma once

#include "Register/SettingUICustomRegister.h"

#include "BESettingUICustomRegister_Keybind.generated.h"

class UBESettingUITypeResolver_Keybind;
class USettingUITypeResolver;
class USettingUISubsystem;


/**
 * キーバインド設定の自動登録を行うためのクラス
 */
UCLASS()
class UBESettingUICustomRegister_Keybind : public USettingUICustomRegister
{
	GENERATED_BODY()
public:
	UBESettingUICustomRegister_Keybind(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	////////////////////////////////////////////////////////////////////////
	// 設定
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
	TSubclassOf<UBESettingUITypeResolver_Keybind> ResolverClass;

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
	TSubclassOf<USettingUITypeResolver> HeaderClass;

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
	bool bShowKeyboardAndMouse{ true };

	UPROPERTY(EditDefaultsOnly, Category = "Setting")
	bool bShowGamepad{ true };

public:
	virtual FRegisteredSettingUITable CreateTable(USettingUISubsystem* Subsystem) const override;

};
