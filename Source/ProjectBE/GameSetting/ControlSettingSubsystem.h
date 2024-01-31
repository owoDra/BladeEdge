// Copyright (C) 2024 owoDra

#pragma once

#include "GSCSubsystem.h"

#include "Input/GamepadSensitivityData.h"

#include "ControlSettingSubsystem.generated.h"


/**
 * コントロール設定
 */
UCLASS(configdonotcheckdefaults)
class PROJECTBE_API UControlSettingSubsystem : public UGSCSubsystem
{
	GENERATED_BODY()
public:
	UControlSettingSubsystem() {}

protected:
	virtual FString GetCustomConfigIniName() const override { return FString(TEXT("UserControlSettings")); }

protected:
	virtual void SetToDefaults() override;


	// ===== 共有系 | 反転設定 =====
private:
	// 縦軸の反転
	UPROPERTY(Config) bool bInvertVerticalAxis{ false };

	// 横軸の反転
	UPROPERTY(Config) bool bInvertHorizontalAxis{ false };

public:
	/**
	 *  縦軸の反転
	 */
	UFUNCTION() bool GetInvertVerticalAxis() const { return bInvertVerticalAxis; }
	UFUNCTION() void SetInvertVerticalAxis(bool NewValue) { ChangeValueAndDirty(bInvertVerticalAxis, NewValue); }

	/**
	 *  横軸の反転
	 */
	UFUNCTION() bool GetInvertHorizontalAxis() const { return bInvertHorizontalAxis; }
	UFUNCTION() void SetInvertHorizontalAxis(bool NewValue) { ChangeValueAndDirty(bInvertHorizontalAxis, NewValue); }


	// ===== キーボードマウス系 =====
private:
	// 横軸へのマウス感度
	UPROPERTY(Config) double MouseSensitivityX{ 1.0f };

	// 縦軸へのマウス感度
	UPROPERTY(Config) double MouseSensitivityY{ 1.0f };

public:
	/**
	 *  横軸へのマウス感度
	 */
	UFUNCTION() double GetMouseSensitivityX() const { return MouseSensitivityX; }
	UFUNCTION() void SetMouseSensitivityX(double NewValue) { ChangeValueAndDirty(MouseSensitivityX, NewValue); }

	/**
	 *  縦軸へのマウス感度
	 */
	UFUNCTION() double GetMouseSensitivityY() const { return MouseSensitivityY; }
	UFUNCTION() void SetMouseSensitivityY(double NewValue) { ChangeValueAndDirty(MouseSensitivityY, NewValue); }


	// ===== ゲームコントローラー系 | 感度設定 =====
private:
	// コントローラーの感度設定
	UPROPERTY(Config) EGamepadSensitivity GamepadLookSensitivityPreset{ EGamepadSensitivity::Normal };

public:
	/**
	 *  コントローラーの感度設定
	 */
	UFUNCTION() EGamepadSensitivity GetGamepadLookSensitivityPreset() const { return GamepadLookSensitivityPreset; }
	UFUNCTION() void SetLookSensitivityPreset(EGamepadSensitivity NewValue) { ChangeValueAndDirty(GamepadLookSensitivityPreset, NewValue); }


	// ===== ゲームコントローラー系 | デッドゾーン設定 =====
private:
	// 移動用のスティックのデッドゾーン
	UPROPERTY(Config) float GamepadMoveStickDeadZone;

	// 視点操作用のスティックのデッドゾーン
	UPROPERTY(Config) float GamepadLookStickDeadZone;

public:
	/**
	 *  移動用のスティックのデッドゾーン
	 */
	UFUNCTION() float GetGamepadMoveStickDeadZone() const { return GamepadMoveStickDeadZone; }
	UFUNCTION() void SetGamepadMoveStickDeadZone(const float NewValue) { ChangeValueAndDirty(GamepadMoveStickDeadZone, NewValue); }

	/**
	 *  視点操作用のスティックのデッドゾーン
	 */
	UFUNCTION() float GetGamepadLookStickDeadZone() const { return GamepadLookStickDeadZone; }
	UFUNCTION() void SetGamepadLookStickDeadZone(const float NewValue) { ChangeValueAndDirty(GamepadLookStickDeadZone, NewValue); }


	// ===== ゲームコントローラー系 | 振動設定 =====
private:
	// 振動を有効にするかどうか
	UPROPERTY(Config) bool bForceFeedbackEnabled{ true };

public:
	/**
	 *  振動設定
	 */
	UFUNCTION() bool GetForceFeedbackEnabled() const { return bForceFeedbackEnabled; }
	UFUNCTION() void SetForceFeedbackEnabled(const bool NewValue) { ChangeValueAndDirty(bForceFeedbackEnabled, NewValue); }


	// ===== ゲームコントローラー系 | 触覚フィードバック設定 =====
private:
	// 触覚フィードバックをするかどうか
	UPROPERTY(Config) bool bTriggerHapticsEnabled{ false };

	// トリガーを引いたときに任意の位置でフィードバックをするかどうか
	UPROPERTY(Config) bool bTriggerPullUsesHapticThreshold{ true };

	// 触覚フィードバックの強さ
	UPROPERTY(Config) uint8 TriggerHapticStrength{ 8 };

	// 触覚フィードバックを行うトリガー位置
	UPROPERTY(Config) uint8 TriggerHapticStartPosition{ 0 };

public:
	/**
	 *  触覚フィードバックの有効化設定
	 */
	UFUNCTION() bool GetTriggerHapticsEnabled() const { return bTriggerHapticsEnabled; }
	UFUNCTION() void SetTriggerHapticsEnabled(const bool NewValue) { ChangeValueAndDirty(bTriggerHapticsEnabled, NewValue); }

	/**
	 *  触覚フィードバックのトリガー位置の有効化設定
	 */
	UFUNCTION() bool GetTriggerPullUsesHapticThreshold() const { return bTriggerPullUsesHapticThreshold; }
	UFUNCTION() void SetTriggerPullUsesHapticThreshold(const bool NewValue) { ChangeValueAndDirty(bTriggerPullUsesHapticThreshold, NewValue); }

	/**
	 *  触覚フィードバックの強度設定
	 */
	UFUNCTION() uint8 GetTriggerHapticStrength() const { return TriggerHapticStrength; }
	UFUNCTION() void SetTriggerHapticStrength(const uint8 NewValue) { ChangeValueAndDirty(TriggerHapticStrength, NewValue); }

	/**
	 *  触覚フィードバックのトリガー位置設定
	 */
	UFUNCTION() uint8 GetTriggerHapticStartPosition() const { return TriggerHapticStartPosition; }
	UFUNCTION() void SetTriggerHapticStartPosition(const uint8 NewValue) { ChangeValueAndDirty(TriggerHapticStartPosition, NewValue); }

};
