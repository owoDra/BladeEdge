// Copyright (C) 2024 owoDra

#include "UserFacingExperienceData.h"

#include "DeveloperSettings/BELobbyDeveloperSettings.h"

#include "Type/OnlineLobbySearchTypes.h"
#include "Type/OnlineLobbyCreateTypes.h"
#include "OnlineLobbySubsystem.h"

#include "Online/OnlineSessionNames.h"
#include "GameFramework/PlayerController.h"

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
		CreateRequest->ModeNameForAdvertisement = ExperienceName;
		CreateRequest->MapID = MapIDs[PickMapIndex];
		CreateRequest->ExtraArgs = ExtraArgs;
		CreateRequest->ExtraArgs.Add(TEXT("ExperienceData"), ExperienceName);
		CreateRequest->MaxPlayerCount = MaxPlayers;
		CreateRequest->InitialAttributes = LobbyAttributes;
		CreateRequest->InitialUserAttributes = LobbyUserAttributes;

		SearchRequest = LobbySubsystem->CreateOnlineLobbySearchRequest();
		FLobbyAttributeFilter ModeNameFilter
		{
			FLobbyAttribute(FName(SETTING_GAMEMODE), ExperienceName),
			ELobbyAttributeComparisonOp::Equals
		};
		SearchRequest->Filters.Add(ModeNameFilter);

		static const FName NAME_MATCHSTARTED{ TEXTVIEW("MATCHSTARTED") };
		FLobbyAttributeFilter MatchStartedFilter
		{
			FLobbyAttribute(NAME_MATCHSTARTED, false),
			ELobbyAttributeComparisonOp::Equals
		};
		SearchRequest->Filters.Add(MatchStartedFilter);

		SearchRequest->Filters.Append(LobbyAttributeFilters);
	}
}

void UUserFacingExperienceData::CreateRequestsForMatchmaking(UOnlineLobbySubsystem* LobbySubsystem, APlayerController* HostingPlayerController, ULobbyCreateRequest*& CreateRequest, ULobbySearchRequest*& SearchRequest) const
{
	if (LobbySubsystem && HostingPlayerController)
	{
		CreateRequestsFromThis(LobbySubsystem, CreateRequest, SearchRequest);

		////////////////////////////////////////
		// グローバルアトリビュートを結合

		const auto* DevSetting{ GetDefault<UBELobbyDeveloperSettings>() };

		CreateRequest->InitialAttributes.Append(DevSetting->GlobalLobbyAttributesForMatchmaking);
		CreateRequest->InitialUserAttributes.Append(DevSetting->GlobalLobbyUserAttributesForMatchmaking);

		SearchRequest->Filters.Append(DevSetting->GlobalLobbyAttributeFiltersForMatchmaking);

		////////////////////////////////////////
		// ユニークID を追加

		// 構築に用いる文字列をトリミングする
		static constexpr int32 NUM_ReferForBuildUniqueId{ 16 };

		auto* LocalPlayer{ HostingPlayerController->GetLocalPlayer() };
		auto AccountId{ ensure(LocalPlayer) ? ToString(LocalPlayer->GetPreferredUniqueNetId().GetV2()) : FString()};
		AccountId = AccountId.Left(NUM_ReferForBuildUniqueId);
		auto GUID{ FGuid::NewGuid().ToString() };
		GUID = GUID.Left(NUM_ReferForBuildUniqueId);

		const auto NewUniqueID{ AccountId + GUID };
		
		CreateRequest->InitialAttributes.Add({ UBELobbyDeveloperSettings::GetLobbyAttrName_UNIQUEID(), NewUniqueID });
	}
}

void UUserFacingExperienceData::CreateRequestsForRejoin(UOnlineLobbySubsystem* LobbySubsystem, const FString& LobbyUniqueID, ULobbyCreateRequest*& CreateRequest, ULobbySearchRequest*& SearchRequest) const
{
	if (LobbySubsystem && !LobbyUniqueID.IsEmpty())
	{
		CreateRequestsFromThis(LobbySubsystem, CreateRequest, SearchRequest);

		////////////////////////////////////////
		// グローバルアトリビュートを結合

		const auto* DevSetting{ GetDefault<UBELobbyDeveloperSettings>() };

		CreateRequest->InitialAttributes.Append(DevSetting->GlobalLobbyAttributesForRejoin);
		CreateRequest->InitialUserAttributes.Append(DevSetting->GlobalLobbyUserAttributesForRejoin);

		SearchRequest->Filters.Append(DevSetting->GlobalLobbyAttributeFiltersForRejoin);

		////////////////////////////////////////
		// ユニークID を追加

		FLobbyAttribute UniqueIdAttr{ UBELobbyDeveloperSettings::GetLobbyAttrName_UNIQUEID(), LobbyUniqueID };
		FLobbyAttributeFilter UniqueIdAttrFilter{ UniqueIdAttr, ELobbyAttributeComparisonOp::Equals };

		CreateRequest->InitialAttributes.Add(UniqueIdAttr);
		SearchRequest->Filters.Add(UniqueIdAttrFilter);
	}
}
