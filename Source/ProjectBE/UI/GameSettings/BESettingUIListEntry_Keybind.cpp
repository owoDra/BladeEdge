// Copyright (C) 2023 owoDra

#include "BESettingUIListEntry_Keybind.h"

#include "GameSetting/Resolver/BESettingUITypeResolver_Keybind.h"
#include "UI/GameSettings/BESettingUIPressAnyKeyWidget.h"

#include "UIFunctionLibrary.h"
#include "GameplayTag/GUIETags_UI.h"

#include "Components/Image.h"
#include "CommonButtonBase.h"
#include "CommonTextBlock.h"
#include "CommonInputBaseTypes.h"
#include "CommonInputSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BESettingUIListEntry_Keybind)

/////////////////////////////////////////////////////////////////////////

UBESettingUIListEntry_Keybind::UBESettingUIListEntry_Keybind(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


// Initialization

void UBESettingUIListEntry_Keybind::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_PrimaryKey->OnClicked().AddUObject(this, &ThisClass::HandlePrimaryKeyClicked);
	Button_SecondaryKey->OnClicked().AddUObject(this, &ThisClass::HandleSecondaryKeyClicked);
	Button_ResetToDefault->OnClicked().AddUObject(this, &ThisClass::HandleResetToDefaultClicked);
}


// Change Setting

void UBESettingUIListEntry_Keybind::HandleSettingValueChanged(USettingUITypeResolver* InSetting)
{
	auto* KeybindResolver{ Cast<UBESettingUITypeResolver_Keybind>(InSetting) };
	if (ensure(KeybindResolver))
	{
		FKey PrimaryKey;
		FKey SecondaryKey;

		KeybindResolver->GetBinds(PrimaryKey, SecondaryKey);

		SetPrimaryKey(PrimaryKey);
		SetSecondaryKey(SecondaryKey);
	}

	Super::HandleSettingValueChanged(InSetting);
}


void UBESettingUIListEntry_Keybind::HandlePrimaryKeyClicked()
{
	auto* PressAnyKeyWidget
	{
		CastChecked<UBESettingUIPressAnyKeyWidget>(
			UUIFunctionLibrary::PushContentToLayer_ForPlayer(GetOwningLocalPlayer(), TAG_UI_Layer_Modal, PressAnyKeyWidgetClass))
	};

	PressAnyKeyWidget->OnKeySelected.AddUObject(this, &ThisClass::HandlePrimaryKeySelected, PressAnyKeyWidget);
	PressAnyKeyWidget->OnKeySelectionCanceled.AddUObject(this, &ThisClass::HandleKeySelectionCanceled, PressAnyKeyWidget);
}

void UBESettingUIListEntry_Keybind::HandleSecondaryKeyClicked()
{
	auto* PressAnyKeyWidget
	{
		CastChecked<UBESettingUIPressAnyKeyWidget>(
			UUIFunctionLibrary::PushContentToLayer_ForPlayer(GetOwningLocalPlayer(), TAG_UI_Layer_Modal, PressAnyKeyWidgetClass))
	};

	PressAnyKeyWidget->OnKeySelected.AddUObject(this, &ThisClass::HandleSecondaryKeySelected, PressAnyKeyWidget);
	PressAnyKeyWidget->OnKeySelectionCanceled.AddUObject(this, &ThisClass::HandleKeySelectionCanceled, PressAnyKeyWidget);
}

void UBESettingUIListEntry_Keybind::HandleResetToDefaultClicked()
{
	auto* KeybindResolver{ GetResolver<UBESettingUITypeResolver_Keybind>() };
	if (ensure(KeybindResolver))
	{
		KeybindResolver->ResetBinds();
	}
}


void UBESettingUIListEntry_Keybind::HandlePrimaryKeySelected(FKey InKey, UBESettingUIPressAnyKeyWidget* PressAnyKeyWidget)
{
	PressAnyKeyWidget->OnKeySelected.RemoveAll(this);
	PressAnyKeyWidget->OnKeySelectionCanceled.RemoveAll(this);

	SetSettingPrimaryKey(InKey);
}

void UBESettingUIListEntry_Keybind::HandleSecondaryKeySelected(FKey InKey, UBESettingUIPressAnyKeyWidget* PressAnyKeyWidget)
{
	PressAnyKeyWidget->OnKeySelected.RemoveAll(this);
	PressAnyKeyWidget->OnKeySelectionCanceled.RemoveAll(this);

	SetSettingSecondaryKey(InKey);
}

void UBESettingUIListEntry_Keybind::HandleKeySelectionCanceled(UBESettingUIPressAnyKeyWidget* PressAnyKeyWidget)
{
	PressAnyKeyWidget->OnKeySelected.RemoveAll(this);
	PressAnyKeyWidget->OnKeySelectionCanceled.RemoveAll(this);
}


void UBESettingUIListEntry_Keybind::SetPrimaryKey(FKey NewKey)
{
	if (NewKey.IsValid())
	{
		if (auto* Resource{ GetIconForKey(NewKey).GetResourceObject() })
		{
			Image_PrimaryKey->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Image_PrimaryKey->SetBrushResourceObject(Resource);
			return;
		}
	}

	Image_PrimaryKey->SetVisibility(ESlateVisibility::Collapsed);
}

void UBESettingUIListEntry_Keybind::SetSecondaryKey(FKey NewKey)
{
	if (NewKey.IsValid())
	{
		if (auto* Resource{ GetIconForKey(NewKey).GetResourceObject() })
		{
			Image_SecondaryKey->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Image_SecondaryKey->SetBrushResourceObject(Resource);
			return;
		}
	}

	Image_SecondaryKey->SetVisibility(ESlateVisibility::Collapsed);
}


void UBESettingUIListEntry_Keybind::SetSettingPrimaryKey(FKey NewKey)
{
	auto* KeybindResolver{ GetResolver<UBESettingUITypeResolver_Keybind>() };
	if (ensure(KeybindResolver))
	{
		KeybindResolver->SetPrimary(NewKey);
	}
}

void UBESettingUIListEntry_Keybind::SetSettingSecondaryKey(FKey NewKey)
{
	auto* KeybindResolver{ GetResolver<UBESettingUITypeResolver_Keybind>() };
	if (ensure(KeybindResolver))
	{
		KeybindResolver->SetSecondary(NewKey);
	}
}


FSlateBrush UBESettingUIListEntry_Keybind::GetIconForKey(FKey InKey) const
{
	FSlateBrush SlateBrush;

	auto* CommonInputSubsystem{ ULocalPlayer::GetSubsystem<UCommonInputSubsystem>(GetOwningLocalPlayer()) };
	auto* InputPlatformSettings{ UCommonInputPlatformSettings::Get() };

	if (CommonInputSubsystem && InputPlatformSettings)
	{
		auto GamepadName{ CommonInputSubsystem->GetCurrentGamepadName() };
		GamepadName = GamepadName.IsNone() ? InputPlatformSettings->GetDefaultGamepadName() : GamepadName;

		InputPlatformSettings->TryGetInputBrush(
			SlateBrush,
			InKey,
			InKey.IsGamepadKey() ? ECommonInputType::Gamepad : ECommonInputType::MouseAndKeyboard,
			GamepadName);
	}

	return SlateBrush;
}
