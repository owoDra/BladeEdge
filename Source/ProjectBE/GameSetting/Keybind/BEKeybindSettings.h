// Copyright (C) 2024 owoDra

#pragma once

#include "Keybind/KeybindSettings.h"

#include "BEKeybindSettings.generated.h"


/**
 * このゲームで使用するInputActionを設定画面に登録するための情報を加えた PlayerMappableKeySettings クラス
 */
UCLASS()
class PROJECTBE_API UBEKeybindSettings : public UKeybindSettings
{
	GENERATED_BODY()

protected:
	//
	// キーボードマウス設定で公開するかどうか
	//
	UPROPERTY(EditAnywhere, Category = "Settings")
	bool bExposeKeyboardAndMouse{ true };

	//
	// ゲームパッド設定で公開するかどうか
	//
	UPROPERTY(EditAnywhere, Category = "Settings")
	bool bExposeGamepad{ true };

public:
	const bool& CanExposeKeyboardAndMouse() const { return bExposeKeyboardAndMouse; }
	const bool& CanExposeGamepad() const { return bExposeGamepad; }

};
