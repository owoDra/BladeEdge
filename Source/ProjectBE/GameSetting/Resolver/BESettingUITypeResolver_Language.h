// Copyright (C) 2024 owoDra

#pragma once

#include "Resolver/SettingUITypeResolver_Switch.h"

#include "BESettingUITypeResolver_Language.generated.h"


/**
 * Resolver class for "Switch(String)" type setting option
 */
UCLASS(Blueprintable, meta = (DisplayName = "Language Switch Resolver"))
class UBESettingUITypeResolver_Switch_Language final : public USettingUITypeResolver_Switch
{
	GENERATED_BODY()
public:
	UBESettingUITypeResolver_Switch_Language(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	inline static int32 SettingSystemDefaultLanguageIndex{ 0 };

	////////////////////////////////////////////////////////////////////////////
	// Initialization
public:
	virtual void OnInitialized() override;


	////////////////////////////////////////////////////////////////////////////
	// Getter・Setter
private:
	UPROPERTY(Transient)
	TArray<FText> OptionDisplayTexts;

	UPROPERTY(Transient)
	TArray<FString> OptionValues;

public:
	virtual void SetSettingValue(int32 NewValue) override;
	virtual int32 GetSettingValue() const override;

	virtual TArray<FText> GetOptionDisplayTexts() const override;

};
