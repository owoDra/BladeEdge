// Copyright (C) 2024 owoDra

#pragma once

#include "EquipmentWidget.h"

// Game Ability Extension
#include "Type/AbilityActivationMessageTypes.h"
#include "Type/AbilityCooldownMessageTypes.h"

#include "SkillWidget.generated.h"


/**
 * スキル装備の情報を追跡する機能がついたウィジェット
 */
UCLASS(Abstract, Blueprintable)
class PROJECTBE_API USkillWidget : public UEquipmentWidget
{
	GENERATED_BODY()
public:
	USkillWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/////////////////////////////////////////////////////////////////////////////////
	// Ability
protected:
	UPROPERTY(EditAnywhere, Category = "Equipment|Activation")
	bool bTrackingActivationMessage{ false };

	UPROPERTY(EditAnywhere, Category = "Equipment|Activation", meta = (EditCondition = "bTrackingActivationMessage", Categories = "Message.Ability.Activation"))
	FGameplayTag ActivationMessageTag;

	UPROPERTY(EditAnywhere, Category = "Equipment|Cooldown")
	bool bTrackingCooldownMessage{ false };

	UPROPERTY(EditAnywhere, Category = "Equipment|Cooldown", meta = (EditCondition = "bTrackingCooldownMessage", Categories = "Message.Ability.Cooldown"))
	FGameplayTag CooldownMessageTag;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Cooldown")
	float CurrentCooldown{ 0.0f };

private:
	UPROPERTY(Transient)
	FGameplayMessageListenerHandle ActivationMessageListenerHandle;

	UPROPERTY(Transient)
	FGameplayMessageListenerHandle CooldownMessageListenerHandle;

protected:
	virtual void ListenMessageEvents() override;
	virtual void UnlistenMessageEvents() override;

	/**
	 * Notifies that ability activation
	 */
	void HandleActivationMessage(FGameplayTag MessageTag, const FAbilityActivationMessage& Message);

	/**
	 * Notifies that ability cooldown
	 */
	void HandleCooldownMessage(FGameplayTag MessageTag, const FAbilityCooldownMessage& Message);

	UFUNCTION(BlueprintNativeEvent, Category = "Equipment")
	void OnActivation();
	virtual void OnActivation_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, Category = "Equipment")
	void OnCooldownStart(float Duration);
	virtual void OnCooldownStart_Implementation(float Duration) {}

	UFUNCTION(BlueprintNativeEvent, Category = "Equipment")
	void OnCooldownEnd();
	virtual void OnCooldownEnd_Implementation() {}

};
