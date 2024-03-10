// Copyright (C) 2024 owoDra

#pragma once

#include "Engine/DeveloperSettings.h"

#include "Type/OnlineLobbyAttributeTypes.h"

#include "BELobbyDeveloperSettings.generated.h"

class FName;


/**
 * このゲームのロビー作成・検索に関するデベロッパー設定
 */
UCLASS(Config = "Game", Defaultconfig, meta = (DisplayName = "Lobby"))
class UBELobbyDeveloperSettings : public UDeveloperSettings
{
public:
	GENERATED_BODY()
public:
	UBELobbyDeveloperSettings();

	///////////////////////////////////////////////
	// Attribute Names
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Lobby Attribute")
	static const FName& GetLobbyAttrName_GAMEMODE()
	{ 
		static const FName NAME_LobbyAttr_GAMEMODE{ TEXTVIEW("GAMEMODE") };
		return NAME_LobbyAttr_GAMEMODE;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Lobby Attribute")
	static const FName& GetLobbyAttrName_MAPNAME()
	{
		static const FName NAME_LobbyAttr_MAPNAME{ TEXTVIEW("MAPNAME") };
		return NAME_LobbyAttr_MAPNAME;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Lobby Attribute")
	static const FName& GetLobbyAttrName_VICTORYPOINT()
	{
		static const FName NAME_LobbyAttr_VICTORYPOINT{ TEXTVIEW("VICTORYPOINT") };
		return NAME_LobbyAttr_VICTORYPOINT;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Lobby Attribute")
	static const FName& GetLobbyAttrName_MATCHTIME()
	{
		static const FName NAME_LobbyAttr_MATCHTIME{ TEXTVIEW("MATCHTIME") };
		return NAME_LobbyAttr_MATCHTIME;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Lobby Attribute")
	static const FName& GetLobbyAttrName_UNIQUEID()
	{
		static const FName NAME_LobbyAttr_UNIQUEID{ TEXTVIEW("UNIQUEID") };
		return NAME_LobbyAttr_UNIQUEID;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Lobby Attribute")
	static const FName& GetLobbyAttrName_MATCHSTARTED()
	{
		static const FName NAME_LobbyAttr_MATCHSTARTED{ TEXTVIEW("MATCHSTARTED") };
		return NAME_LobbyAttr_MATCHSTARTED;
	}


	///////////////////////////////////////////////
	// Matchmaking
public:
	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere, Category = "Matchmaking")
	TSet<FLobbyAttribute> GlobalLobbyAttributesForMatchmaking;

	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere, Category = "Matchmaking")
	TSet<FLobbyAttribute> GlobalLobbyUserAttributesForMatchmaking;

	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere, Category = "Matchmaking")
	TSet<FLobbyAttributeFilter> GlobalLobbyAttributeFiltersForMatchmaking;


	///////////////////////////////////////////////
	// Rejoin
public:
	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere, Category = "ReJoin")
	TSet<FLobbyAttribute> GlobalLobbyAttributesForRejoin;

	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere, Category = "ReJoin")
	TSet<FLobbyAttribute> GlobalLobbyUserAttributesForRejoin;

	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere, Category = "ReJoin")
	TSet<FLobbyAttributeFilter> GlobalLobbyAttributeFiltersForRejoin;

};

