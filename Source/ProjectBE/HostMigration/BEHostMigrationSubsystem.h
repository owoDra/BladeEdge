// Copyright (C) 2024 owoDra

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"

#include "BEHostMigrationSubsystem.generated.h"

class ULobbyCreateRequest;


/**
 * ホストマイグレーションの必要の有無を管理するサブシステム
 */
UCLASS(BlueprintType)
class PROJECTBE_API UBEHostMigrationSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
   UBEHostMigrationSubsystem() {}

    ///////////////////////////////////////////////////////////////////////
    // Initialization
public:
    virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    ///////////////////////////////////////////////////////////////////////
    // HostMigration
protected:
	UPROPERTY(Transient)
    TObjectPtr<ULobbyCreateRequest> MigrationLobbyRequest{ nullptr };

public:
    virtual void SetMigrationLobbyRequest(ULobbyCreateRequest* InReuqest);

	UFUNCTION(BlueprintCallable, Category = "HostMigration")
    virtual void ClearMigrationLobbyRequest();

    UFUNCTION(BlueprintCallable, Category = "HostMigration")
    virtual ULobbyCreateRequest* GetMigrationLobbyRequest() const { return MigrationLobbyRequest; }

    UFUNCTION(BlueprintCallable, Category = "HostMigration", meta = (ExpandBoolAsExecs = "ReturnValue"))
    virtual bool HasMigrationLobbyRequest() const;

};
