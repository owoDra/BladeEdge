// Copyright (C) 2024 owoDra

#include "BELobbyDeveloperSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BELobbyDeveloperSettings)


UBELobbyDeveloperSettings::UBELobbyDeveloperSettings()
{
	CategoryName = TEXT("ProjectBE");
	SectionName = TEXT("Lobby");

	// Matchmaking
	{
		GlobalLobbyAttributesForMatchmaking.Empty();
		GlobalLobbyUserAttributesForMatchmaking.Empty();
		GlobalLobbyAttributeFiltersForMatchmaking.Empty();
		const FLobbyAttribute MatchStartedAttr
		{
			UBELobbyDeveloperSettings::GetLobbyAttrName_MATCHSTARTED(),
			false
		};
		const FLobbyAttributeFilter MatchStartedAttrFilter
		{
			MatchStartedAttr,
			ELobbyAttributeComparisonOp::Equals
		};
		GlobalLobbyAttributesForMatchmaking.Add(MatchStartedAttr);
		GlobalLobbyAttributeFiltersForMatchmaking.Add(MatchStartedAttrFilter);
	}

	// Rejoin
	{
		GlobalLobbyAttributesForRejoin.Empty();
		GlobalLobbyUserAttributesForRejoin.Empty();
		GlobalLobbyAttributeFiltersForRejoin.Empty();
		const FLobbyAttribute MatchStartedAttr
		{
			UBELobbyDeveloperSettings::GetLobbyAttrName_MATCHSTARTED(),
			true
		};
		const FLobbyAttributeFilter MatchStartedAttrFilter
		{
			MatchStartedAttr,
			ELobbyAttributeComparisonOp::Equals
		};
		GlobalLobbyAttributesForRejoin.Add(MatchStartedAttr);
		GlobalLobbyAttributeFiltersForRejoin.Add(MatchStartedAttrFilter);
	}
}
