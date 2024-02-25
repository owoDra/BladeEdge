// Copyright (C) 2024 owoDra

#include "UserFacingExperienceData.h"

#include "Type/OnlineLobbySearchTypes.h"
#include "Type/OnlineLobbyCreateTypes.h"
#include "OnlineLobbySubsystem.h"

#include "Online/OnlineSessionNames.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(UserFacingExperienceData)

//////////////////////////////////////////////////////////////////////////

UUserFacingExperienceData::UUserFacingExperienceData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


#if WITH_EDITOR
EDataValidationResult UUserFacingExperienceData::IsDataValid(FDataValidationContext& Context) const
{
	auto Result{ CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid) };

	if (MapIDs.IsEmpty())
	{
		Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);

		Context.AddError(FText::FromString(FString::Printf(TEXT("No MapId in MapIds in %s"), *GetNameSafe(this))));
	}

	auto EntryIndex{ 0 };
	for (const auto& MapID : MapIDs)
	{
		if (!MapID.IsValid())
		{
			Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);

			Context.AddError(FText::FromString(FString::Printf(TEXT("Invalid MapId defined in MapIds[%d] in %s"), EntryIndex, *GetNameSafe(this))));
		}

		++EntryIndex;
	}

	if (!ExperienceID.IsValid())
	{
		Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);

		Context.AddError(FText::FromString(FString::Printf(TEXT("Invalid Experience defined in %s"), *GetNameSafe(this))));
	}

#if WITH_EDITORONLY_DATA
	PrimaryAssetId = GetIdentifierString();
#endif

	return Result;
}
#endif


FPrimaryAssetId UUserFacingExperienceData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(FPrimaryAssetType(UUserFacingExperienceData::NAME_UserFacingExperienceDataType), GetFName());
}

FString UUserFacingExperienceData::GetIdentifierString() const
{
	return GetPrimaryAssetId().ToString();
}


// Experience Info

void UUserFacingExperienceData::CreateRequestsFromThis(UOnlineLobbySubsystem* LobbySubsystem, ULobbyCreateRequest*& CreateRequest, ULobbySearchRequest*& SearchRequest) const
{
	if (LobbySubsystem)
	{
		const auto PickMapIndex{ FMath::RandRange(0, MapIDs.Num() - 1) };
		const auto ExperienceName{ ExperienceID.PrimaryAssetName.ToString() };
		const auto UserFacingExperienceName{ GetPrimaryAssetId().PrimaryAssetName.ToString() };

		CreateRequest = LobbySubsystem->CreateOnlineLobbyCreateRequest();
		CreateRequest->bPresenceEnabled = bPresenceEnabled;
		CreateRequest->ModeNameForAdvertisement = UserFacingExperienceName;
		CreateRequest->MapID = MapIDs[PickMapIndex];
		CreateRequest->ExtraArgs = ExtraArgs;
		CreateRequest->ExtraArgs.Add(TEXT("ExperienceData"), ExperienceName);
		CreateRequest->MaxPlayerCount = MaxPlayers;

		SearchRequest = LobbySubsystem->CreateOnlineLobbySearchRequest();
		FLobbyAttributeFilter ModeNameFilter
		{
			FLobbyAttribute(FName(SETTING_GAMEMODE), UserFacingExperienceName),
			ELobbyAttributeComparisonOp::Equals
		};
		SearchRequest->Filters.Add(ModeNameFilter);
	}
}
