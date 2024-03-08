// Copyright (C) 2024 owoDra

#include "BEHostMigrationSubsystem.h"

#include "Type/OnlineLobbyCreateTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEHostMigrationSubsystem)


// Initialization

bool UBEHostMigrationSubsystem::ShouldCreateSubsystem(UObject* Outer) const
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

void UBEHostMigrationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
}

void UBEHostMigrationSubsystem::Deinitialize()
{
	ClearMigrationLobbyRequest();
}


void UBEHostMigrationSubsystem::SetMigrationLobbyRequest(ULobbyCreateRequest* InReuqest)
{
	MigrationLobbyRequest = InReuqest;
}

void UBEHostMigrationSubsystem::ClearMigrationLobbyRequest()
{
	MigrationLobbyRequest = nullptr;
}

bool UBEHostMigrationSubsystem::HasMigrationLobbyRequest() const
{
	return IsValid(MigrationLobbyRequest);
}
