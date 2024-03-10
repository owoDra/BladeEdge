﻿// Copyright (C) 2024 owoDra

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

    void HandleBecomeLeader(FName LocalName);

    ///////////////////////////////////////////////////////////////////////
    // HostMigration
protected:
    //
    // ホストマイグレーションリクエストを受けた際に新規ホスト用のURL
    //
    FString HostMigrationRequestURL;

    //
    // ロビーの新規リーダーになった際にホストマイグレーションが必要にな場合に true になる
    //
    bool bRequested{ false };

public:
    virtual bool HandleHostMigrationRequest();
    virtual bool HandleHostMigrationRequestURL(const FString& InURL);

    UFUNCTION(BlueprintCallable, Category = "HostMigration")
    virtual bool TryHostMigration();

	UFUNCTION(BlueprintCallable, Category = "HostMigration")
    virtual void ClearHostMigrationRequest();

};
