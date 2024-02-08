// Copyright (C) 2024 owoDra

#include "BESettingUIPressAnyKeyWidget.h"

#include "Framework/Application/IInputProcessor.h"
#include "Framework/Application/SlateApplication.h"
#include "CommonInputSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BESettingUIPressAnyKeyWidget)


////////////////////////////////////////////////////////////////////////
// FSettingUIPressAnyKeyInputPreProcessor

class FSettingUIPressAnyKeyInputPreProcessor : public IInputProcessor
{
public:
	FSettingUIPressAnyKeyInputPreProcessor() {}

public:
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override {}

	virtual bool HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		HandleKey(InKeyEvent.GetKey());
		return true;
	}

	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		return true;
	}

	virtual bool HandleMouseButtonDoubleClickEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		HandleKey(MouseEvent.GetEffectingButton());
		return true;
	}

	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		return true;
	}

	virtual bool HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		HandleKey(MouseEvent.GetEffectingButton());
		return true;
	}

	virtual bool HandleMouseWheelOrGestureEvent(FSlateApplication& SlateApp, const FPointerEvent& InWheelEvent, const FPointerEvent* InGestureEvent) override
	{
		if (InWheelEvent.GetWheelDelta() != 0)
		{
			const FKey Key = InWheelEvent.GetWheelDelta() < 0 ? EKeys::MouseScrollDown : EKeys::MouseScrollUp;
			HandleKey(Key);
		}
		return true;
	}

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FSettingsPressAnyKeyInputPreProcessorKeySelected, FKey);
	FSettingsPressAnyKeyInputPreProcessorKeySelected OnKeySelected;

private:
	void HandleKey(const FKey& Key)
	{
		OnKeySelected.Broadcast(Key);
	}
};


////////////////////////////////////////////////////////////////////////
// UBESettingUIPressAnyKeyWidget

UBESettingUIPressAnyKeyWidget::UBESettingUIPressAnyKeyWidget(const FObjectInitializer& Initializer)
	: Super(Initializer)
{
	IgnoreKeys =
	{
		EKeys::LeftCommand,
		EKeys::RightCommand,
	};

	CancelKeys =
	{
		EKeys::Escape,
		EKeys::Gamepad_Special_Right,
	};

	RemoveKeys =
	{
		EKeys::BackSpace,
		EKeys::Gamepad_Special_Left,
	};
}


void UBESettingUIPressAnyKeyWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	bKeySelected = false;

	InputProcessor = MakeShared<FSettingUIPressAnyKeyInputPreProcessor>();
	InputProcessor->OnKeySelected.AddUObject(this, &ThisClass::HandleKey);
	FSlateApplication::Get().RegisterInputPreProcessor(InputProcessor, 0);

	if (auto* InputSubsystem{ GetInputSubsystem() })
	{
		InputSubsystem->OnInputMethodChangedNative.AddUObject(this, &ThisClass::HandleInputMethodChanged);

		HandleInputMethodChanged(InputSubsystem->GetCurrentInputType());
	}
}

void UBESettingUIPressAnyKeyWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(InputProcessor);
	}

	if (auto* InputSubsystem{ GetInputSubsystem() })
	{
		InputSubsystem->OnInputMethodChangedNative.RemoveAll(this);
	}
}


void UBESettingUIPressAnyKeyWidget::HandleInputMethodChanged(ECommonInputType NewInputMethod)
{
	OnInputMethodChanged(NewInputMethod);
}


void UBESettingUIPressAnyKeyWidget::HandleKey(FKey InKey)
{
	// 無視するキーだった場合は何もしない

	if (IgnoreKeys.Contains(InKey))
	{
		return;
	}

	// キーボードキーが許可されていない状態で入力された場合はキャンセル

	if (!bAllowKeyboardAndMouse)
	{
		if (!InKey.IsGamepadKey() && !InKey.IsTouch())
		{
			HandleKeySelectionCanceled();
			return;
		}
	}

	// パッドが許可されていない状態で入力された場合はキャンセル

	if (!bAllowGamepad)
	{
		if (InKey.IsGamepadKey())
		{
			HandleKeySelectionCanceled();
			return;
		}
	}

	// キャンセルキーだった場合はキャンセル

	if (CancelKeys.Contains(InKey))
	{
		HandleKeySelectionCanceled();
		return;
	}

	// 削除キーだった場合はキャンセル

	if (RemoveKeys.Contains(InKey))
	{
		HandleKeySelected(FKey());
		return;
	}

	HandleKeySelected(InKey);
}

void UBESettingUIPressAnyKeyWidget::HandleKeySelected(FKey InKey)
{
	if (!bKeySelected)
	{
		bKeySelected = true;

		Dismiss(
			[this, InKey]() 
			{
				OnKeySelected.Broadcast(InKey);
			}
		);
	}
}

void UBESettingUIPressAnyKeyWidget::HandleKeySelectionCanceled()
{
	if (!bKeySelected)
	{
		bKeySelected = true;

		Dismiss(
			[this]() 
			{
				OnKeySelectionCanceled.Broadcast();
			}
		);
	}
}


void UBESettingUIPressAnyKeyWidget::Dismiss(TFunction<void()> PostDismissCallback)
{
	// We delay a tick so that we're done processing input.

	FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateWeakLambda(this, 
		[this, PostDismissCallback](float DeltaTime)
		{
			QUICK_SCOPE_CYCLE_COUNTER(STAT_UBESettingUIPressAnyKeyWidget_Dismiss);

			FSlateApplication::Get().UnregisterInputPreProcessor(InputProcessor);

			DeactivateWidget();

			PostDismissCallback();

			return false;
		}
	));
}
