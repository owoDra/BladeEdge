// Copyright (C) 2024 owoDra

#pragma once

#include "Foundation/ActivatableWidget.h"

#include "BESettingUIPressAnyKeyWidget.generated.h"

class FSettingUIPressAnyKeyInputPreProcessor;
struct FKey;


/**
 * キーバインド変更時の変更するキーを待つ処理と表示を行うウィジェット
 */
UCLASS(Abstract)
class PROJECTBE_API UBESettingUIPressAnyKeyWidget : public UActivatableWidget
{
	GENERATED_BODY()
public:
	UBESettingUIPressAnyKeyWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	////////////////////////////////////////////////////////////////////////////
	// Wait Input
public:
	DECLARE_EVENT_OneParam(UBESettingUIPressAnyKeyWidget, FOnKeySelected, FKey);
	FOnKeySelected OnKeySelected;

	DECLARE_EVENT(UBESettingUIPressAnyKeyWidget, FOnKeySelectionCanceled);
	FOnKeySelectionCanceled OnKeySelectionCanceled;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "PressAnyKey")
	TSet<FKey> IgnoreKeys;

	UPROPERTY(EditDefaultsOnly, Category = "PressAnyKey")
	TSet<FKey> CancelKeys;

	UPROPERTY(EditDefaultsOnly, Category = "PressAnyKey")
	TSet<FKey> RemoveKeys;

	UPROPERTY(EditDefaultsOnly, Category = "PressAnyKey")
	bool bAllowKeyboardAndMouse{ false };

	UPROPERTY(EditDefaultsOnly, Category = "PressAnyKey")
	bool bAllowGamepad{ false };

	UPROPERTY(Transient)
	bool bKeySelected{ false };

	TSharedPtr<FSettingUIPressAnyKeyInputPreProcessor> InputProcessor;

protected:
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "PressAnyKey")
	void OnInputMethodChanged(ECommonInputType NewInputMethod);
	void HandleInputMethodChanged(ECommonInputType NewInputMethod);

	void HandleKey(FKey InKey);
	void HandleKeySelected(FKey InKey);
	void HandleKeySelectionCanceled();

	void Dismiss(TFunction<void()> PostDismissCallback);

};
