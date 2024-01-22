// Copyright (C) 2024 owoDra

#include "SkillWidget.h"

// Game Equipment Extension
#include "Equipment/Equipment.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SkillWidget)


USkillWidget::USkillWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


void USkillWidget::ListenMessageEvents()
{
	Super::ListenMessageEvents();

	if (const auto* World{ GetWorld() })
	{
		auto& Subsystem{ UGameplayMessageSubsystem::Get(World) };

		if (bTrackingActivationMessage && ActivationMessageTag.IsValid())
		{
			ActivationMessageListenerHandle = Subsystem.RegisterListener<FAbilityActivationMessage>(ActivationMessageTag, this, &ThisClass::HandleActivationMessage);
		}

		if (bTrackingCooldownMessage && CooldownMessageTag.IsValid())
		{
			CooldownMessageListenerHandle = Subsystem.RegisterListener<FAbilityCooldownMessage>(CooldownMessageTag, this, &ThisClass::HandleCooldownMessage);
		}
	}
}

void USkillWidget::UnlistenMessageEvents()
{
	if (ActivationMessageListenerHandle.IsValid())
	{
		ActivationMessageListenerHandle.Unregister();
	}

	if (CooldownMessageListenerHandle.IsValid())
	{
		CooldownMessageListenerHandle.Unregister();
	}

	Super::UnlistenMessageEvents();
}


void USkillWidget::HandleActivationMessage(FGameplayTag MessageTag, const FAbilityActivationMessage& Message)
{
	if (Message.SourceObject == Equipment)
	{
		OnActivation();
	}
}

void USkillWidget::HandleCooldownMessage(FGameplayTag MessageTag, const FAbilityCooldownMessage& Message)
{
	if (Message.SourceObject == Equipment)
	{
		CurrentCooldown = Message.Duration;

		if (CurrentCooldown > 0.0f)
		{
			OnCooldownStart(CurrentCooldown);
		}
		else
		{
			OnCooldownEnd();
		}
	}
}
