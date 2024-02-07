// Copyright (C) 2024 owoDra

#pragma once


UENUM(BlueprintType)
enum class EGamepadSensitivity : uint8
{
	Invalid = 0		UMETA(Hidden),

	Slow			UMETA(DisplayName = "Slow"),
	SlowPlus		UMETA(DisplayName = "Slow+"),
	SlowPlusPlus	UMETA(DisplayName = "Slow++"),
	Normal			UMETA(DisplayName = "Normal"),
	NormalPlus		UMETA(DisplayName = "Normal+"),
	NormalPlusPlus	UMETA(DisplayName = "Normal++"),
	Fast			UMETA(DisplayName = "Fast"),
	FastPlus		UMETA(DisplayName = "Fast+"),
	FastPlusPlus	UMETA(DisplayName = "Fast++"),
	Insane			UMETA(DisplayName = "Insane"),

	MAX				UMETA(Hidden),
};
