// Copyright (C) 2024 owoDra

#pragma once

#include "GSCSubsystem.h"

#include "GameplaySettingSubsystem.generated.h"


/**
 * 色覚特性タイプ
 */
UENUM(BlueprintType)
enum class EColorBlindMode : uint8
{
	Off,

	// Deuteranope (green weak/blind)
	Deuteranope,

	// Protanope (red weak/blind)
	Protanope,

	// Tritanope(blue weak / bind)
	Tritanope
};


/**
 * ゲームプレイ設定
 */
UCLASS(configdonotcheckdefaults)
class PROJECTBE_API UGameplaySettingSubsystem : public UGSCSubsystem
{
	GENERATED_BODY()
public:
	UGameplaySettingSubsystem();

protected:
	virtual FString GetCustomConfigIniName() const override { return FString(TEXT("UserGameplaySettings")); }

protected:
	virtual void SetToDefaults() override;
	virtual void ApplySettings() override;


	// ===== アクセシビリティ系 | 言語設定 =====
private:
	// 適用しようとしている言語
	UPROPERTY(Transient)
	FString PendingCulture;

	// true ならばデフォルトの言語に戻す。
	bool bResetToDefaultCulture{ false };

public:
	UFUNCTION() const FString& GetPendingCulture() const { return PendingCulture; }

	UFUNCTION() void SetPendingCulture(const FString& NewCulture) { ChangeValueAndDirty(PendingCulture, NewCulture); bResetToDefaultCulture = false; ApplyCultureSettings(); }

	void ResetToDefaultCulture();
	bool ShouldResetToDefaultCulture() const { return bResetToDefaultCulture; }

	bool IsUsingDefaultCulture() const;

protected:
	void OnCultureChanged();

	void ClearPendingCulture();

	void ApplyCultureSettings();


	// ===== アクセシビリティ系 | 色覚特性設定 =====
private:
	// 色覚特性の種類
	UPROPERTY(Config) EColorBlindMode ColorBlindMode{ EColorBlindMode::Off };

	// 色覚特性の強さ
	UPROPERTY(Config) int32 ColorBlindStrength{ 10 };

public:
	/**
	 *  色覚特性設定を適用させる
	 */
	void ApplyColorBlindSettings();

	/**
	 *  色覚特性の種類
	 */
	UFUNCTION() EColorBlindMode GetColorBlindMode() const { return ColorBlindMode; }
	UFUNCTION() void SetColorBlindMode(EColorBlindMode InMode) { ChangeValueAndDirty(ColorBlindMode, InMode); ApplyColorBlindSettings(); }

	/**
	 *  色覚特性の強さ
	 */
	UFUNCTION() int32 GetColorBlindStrength() const { return ColorBlindStrength; }
	UFUNCTION() void SetColorBlindStrength(int32 InColorBlindStrength) { ChangeValueAndDirty(ColorBlindStrength, InColorBlindStrength); ApplyColorBlindSettings(); }


	// ===== 視点系 | FOV設定 =====
private:
	// FOV の値
	UPROPERTY(Config) float FieldOfView { 90.0f };

public:
	/**
	 * FOV の値
	 */
	UFUNCTION() float GetFieldOfView() const { return FieldOfView; }
	UFUNCTION() void SetFieldOfView(float InFOV) { ChangeValueAndDirty(FieldOfView, InFOV); }

};
