// Copyright (C) 2024 owoDra

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"

#include "BERejoinMatchSubsystem.generated.h"

class UUserFacingExperienceData;
class ULobbyResult;


/**
 * 現在進行中の対戦から切断されてしまった際に復帰するための情報を管理するサブシステム
 */
UCLASS(BlueprintType)
class PROJECTBE_API UBERejoinMatchSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
   UBERejoinMatchSubsystem() {}

    ///////////////////////////////////////////////////////////////////////
    // Initialization
public:
    virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;


    ///////////////////////////////////////////////////////////////////////
    // RejoinMatch
protected:
	UPROPERTY(Transient)
    FString OngoingLobbyUniqueId;

    UPROPERTY(Transient)
    TObjectPtr<const UUserFacingExperienceData> OngoingLobbyUserFacingExperience;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "RejoinMatch", meta = (ExpandBoolAsExecs = "ReturnValue"))
    virtual bool HasOngoingLobby() const;

    UFUNCTION(BlueprintCallable, Category = "RejoinMatch")
    virtual void ResetOngoingLobby();

    UFUNCTION(BlueprintCallable, Category = "RejoinMatch")
    virtual const UUserFacingExperienceData* GetOngoingLobbyUserFacingExperience() const { return OngoingLobbyUserFacingExperience; }

    UFUNCTION(BlueprintCallable, Category = "RejoinMatch")
    virtual const FString& GetOngoingLobbyUniqueId() const { return OngoingLobbyUniqueId; }

public:
    UFUNCTION(BlueprintCallable, Category = "RejoinMatch")
    virtual void CacheOngoingLobby(const ULobbyResult* InLobbyResult, const UUserFacingExperienceData* InData);

protected:
    virtual void SaveCache();
    virtual void LoadCache();

};
