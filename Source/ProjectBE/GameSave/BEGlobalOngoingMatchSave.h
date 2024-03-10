// Copyright (C) 2024 owoDra

#pragma once

#include "GlobalSave.h"

#include "Type/OnlineLobbyResultTypes.h"

#include "BEGlobalOngoingMatchSave.generated.h"


/**
 * 対戦が進行中の直近のロビーIDを保存するセーブデータ
 */
UCLASS(NotBlueprintable)
class PROJECTBE_API UBEGlobalOngoingMatchSave : public UGlobalSave
{
public:
	GENERATED_BODY()
public:
	UBEGlobalOngoingMatchSave() {}

	virtual FString GetSaveSlotName_Implementation() const override { return TEXT("OngoingMatch"); }

	/////////////////////////////////////////////////////////////////////////////////////
	// セーブデータ
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FPrimaryAssetId OngoingLobbyUserfacingExperience;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lobby")
	FString OngoingLobbyUniqueId;

	/////////////////////////////////////////////////////////////////////////////////////
	// 初期化など
public:
	virtual void ResetToDefault() override;

};

