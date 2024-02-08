// Copyright (C) 2024 owoDra

#pragma once

#include "Keybind/UserKeybindSettings.h"

#include "BEUserKeybindSettings.generated.h"


/** 
 * このプロジェクトにおいてプレイヤーのキーバインドを管理するクラス
 */
UCLASS()
class PROJECTBE_API UBEUserKeybindSettings : public UUserKeybindSettings
{
	GENERATED_BODY()

protected:
	virtual void OnKeyMappingRegistered(FPlayerKeyMapping& RegisteredMapping, const FEnhancedActionKeyMapping& SourceMapping) override;

};
