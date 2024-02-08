// Copyright (C) 2023 owoDra

#pragma once

#include "Widget/ListView/Entry/SettingUIListEntry.h"

#include "BESettingUIListEntry_Keybind.generated.h"

class UBESettingUIPressAnyKeyWidget;
class UCommonButtonBase;
class UImage;


/**
 * キーバインドの設定項目のリストエントリーウィジェットクラス
 */
UCLASS(Abstract, Blueprintable, meta = (DisplayName = "Keybind Setting Entry"))
class PROJECTBE_API UBESettingUIListEntry_Keybind : public USettingUIListEntry
{
	GENERATED_BODY()
public:
	UBESettingUIListEntry_Keybind(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	////////////////////////////////////////////////////////////
	// Initialization
protected:
	virtual void NativeOnInitialized() override;


	////////////////////////////////////////////////////////////
	// Change Setting
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
	TSubclassOf<UBESettingUIPressAnyKeyWidget> PressAnyKeyWidgetClass{ nullptr };


protected:
	virtual void HandleSettingValueChanged(USettingUITypeResolver* InSetting) override;
	 
	void HandlePrimaryKeyClicked();
	void HandleSecondaryKeyClicked();
	void HandleResetToDefaultClicked();

	void HandlePrimaryKeySelected(FKey InKey, UBESettingUIPressAnyKeyWidget* PressAnyKeyWidget);
	void HandleSecondaryKeySelected(FKey InKey, UBESettingUIPressAnyKeyWidget* PressAnyKeyWidget);
	void HandleKeySelectionCanceled(UBESettingUIPressAnyKeyWidget* PressAnyKeyWidget);

	virtual void SetPrimaryKey(FKey NewKey);
	virtual void SetSecondaryKey(FKey NewKey);

	virtual void SetSettingPrimaryKey(FKey NewKey);
	virtual void SetSettingSecondaryKey(FKey NewKey);

	virtual FSlateBrush GetIconForKey(FKey InKey) const;


	////////////////////////////////////////////////////////////
	// Widget Bounds
private:	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UCommonButtonBase> Button_PrimaryKey;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UCommonButtonBase> Button_SecondaryKey;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UCommonButtonBase> Button_ResetToDefault;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UImage> Image_PrimaryKey;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UImage> Image_SecondaryKey;

};