// Copyright (C) 2024 owoDra

#include "BERejoinMatchSubsystem.h"

#include "Experience/UserFacingExperienceData.h"
#include "GameSave/BEGlobalOngoingMatchSave.h"
#include "DeveloperSettings/BELobbyDeveloperSettings.h"
#include "ProjectBELogs.h"

// Game Framework Core
#include "AssetManager/GFCAssetManager.h"

// Game Save Core
#include "GlobalSave/GlobalSaveSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BERejoinMatchSubsystem)


// Initialization

bool UBERejoinMatchSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (Cast<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		return false;
	}

	TArray<UClass*> ChildClasses;
	GetDerivedClasses(GetClass(), ChildClasses, false);

	// Only create an instance if there is not a game-specific subclass

	return ChildClasses.Num() == 0;
}

void UBERejoinMatchSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	auto* GlobalSaveSubsystem{ Collection.InitializeDependency<UGlobalSaveSubsystem>() };
	check(GlobalSaveSubsystem);

	LoadCache();
}

void UBERejoinMatchSubsystem::Deinitialize()
{
}


// RejoinMatch

bool UBERejoinMatchSubsystem::HasOngoingLobby() const
{
	return !OngoingLobbyUniqueId.IsEmpty() && OngoingLobbyUserFacingExperience;
}

void UBERejoinMatchSubsystem::ResetOngoingLobby()
{
	OngoingLobbyUniqueId.Empty();
	OngoingLobbyUserFacingExperience = nullptr;

	SaveCache();
}

void UBERejoinMatchSubsystem::CacheOngoingLobby(const ULobbyResult* InLobbyResult, const UUserFacingExperienceData* InData)
{
	if (ensure(InLobbyResult) && ensure(InData))
	{
		InLobbyResult->GetLobbyAttributeAsString(UBELobbyDeveloperSettings::GetLobbyAttrName_UNIQUEID(), OngoingLobbyUniqueId);

		OngoingLobbyUserFacingExperience = InData;

		UE_LOG(LogBE_RejoinMatch, Log, TEXT("CacheOngoingLobby"))
		UE_LOG(LogBE_RejoinMatch, Log, TEXT("| LobbyResult: %s"), *GetNameSafe(InLobbyResult))
		UE_LOG(LogBE_RejoinMatch, Log, TEXT("| UserFacingExperienceData: %s"), *GetNameSafe(InData))
		UE_LOG(LogBE_RejoinMatch, Log, TEXT("| UniqueId: %s"), *OngoingLobbyUniqueId)
	}

	SaveCache();
}

void UBERejoinMatchSubsystem::SaveCache()
{
	if (auto* GlobalSaveSubsystem{ UGameInstance::GetSubsystem<UGlobalSaveSubsystem>(GetGameInstance()) })
	{
		if (auto* Save{ GlobalSaveSubsystem->GetSave<UBEGlobalOngoingMatchSave>() })
		{
			Save->OngoingLobbyUniqueId = OngoingLobbyUniqueId;
			Save->OngoingLobbyUserfacingExperience = OngoingLobbyUserFacingExperience ? OngoingLobbyUserFacingExperience->GetPrimaryAssetId() : FPrimaryAssetId();
		
			GlobalSaveSubsystem->SyncSaveGameToSlot(UBEGlobalOngoingMatchSave::StaticClass(), FString());
		}
	}
}

void UBERejoinMatchSubsystem::LoadCache()
{
	if (auto* GlobalSaveSubsystem{ UGameInstance::GetSubsystem<UGlobalSaveSubsystem>(GetGameInstance()) })
	{
		if (auto* Save{ GlobalSaveSubsystem->GetSave<UBEGlobalOngoingMatchSave>() })
		{
			OngoingLobbyUniqueId = Save->OngoingLobbyUniqueId;
			OngoingLobbyUserFacingExperience = UGFCAssetManager::GetAsset<UUserFacingExperienceData>(Save->OngoingLobbyUserfacingExperience);
		}
	}
}
