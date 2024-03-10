// Copyright (C) 2024 owoDra

#pragma once

#include "Engine/DataAsset.h"

#include "Type/OnlineLobbyAttributeTypes.h"

#include "UserFacingExperienceData.generated.h"

class UExperienceData;
class ULobbyCreateRequest;
class ULobbySearchRequest;
class UOnlineLobbySubsystem;
class APlayerController;


/**
 * ゲームモードの情報とマッチメイキング開始のためのリクエストを作成するデータアセット
 */
UCLASS(BlueprintType, Const)
class PROJECTBE_API UUserFacingExperienceData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UUserFacingExperienceData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	inline static const FName NAME_UserFacingExperienceDataType{ TEXTVIEW("UserFacingExperienceData") };

	/////////////////////////////////////////////////////////////////////////////////////
	// Validate Data
public:
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif 

	/////////////////////////////////////////////////////////////////////////////////////
	// Primary Asset
protected:
#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, Transient, Category = "Debug")
	mutable FString PrimaryAssetId;
#endif

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	/**
	 * Return PrimaryAssetId as FString
	 */
	UFUNCTION(BlueprintCallable)
	FString GetIdentifierString() const;


	/////////////////////////////////////////////////////////////////////////////////////
	// Experience Info
protected:
	//
	// このゲームモードにおいて遷移可能なワールドリスト
	//
	UPROPERTY(EditDefaultsOnly, Category = "Info", meta = (AllowedTypes = "Map"))
	TArray<FPrimaryAssetId> MapIDs;

	//
	// このゲームモードで使用する ExperienceData のアセットId
	//
	UPROPERTY(EditDefaultsOnly, Category = "Info", meta = (AllowedTypes = "ExperienceData"))
	FPrimaryAssetId ExperienceID;

	//
	// 特定の追加情報がある場合に使用
	//
	UPROPERTY(EditDefaultsOnly, Category = "Info", meta = (ForceInlineRow))
	TMap<FString, FString> ExtraArgs;

	//
	// 特定の追加ロビーアトリビュートがある場合に使用
	//
	UPROPERTY(EditDefaultsOnly, Category = "Info")
	TSet<FLobbyAttribute> LobbyAttributes;

	//
	// 特定の追加ロビーユーザーアトリビュートがある場合に使用
	//
	UPROPERTY(EditDefaultsOnly, Category = "Info")
	TSet<FLobbyAttribute> LobbyUserAttributes;

	//
	// 特定の検索用ロビーアトリビュートがある場合に使用
	//
	UPROPERTY(EditDefaultsOnly, Category = "Info")
	TSet<FLobbyAttributeFilter> LobbyAttributeFilters;

	//
	// このゲームモードの最大人数
	//
	UPROPERTY(EditDefaultsOnly, Category = "Info")
	int32 MaxPlayers;

	//
	// プレゼンスに公開するかどうか
	//
	UPROPERTY(EditDefaultsOnly, Category = "Info")
	bool bPresenceEnabled{ true };

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "User Facing")
	FText DisplayName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "User Facing")
	FText Description;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "User Facing", meta = (DisplayThumbnail = "true", AssetBundles = "Client", AllowedClasses = "/Script/Engine.Texture,/Script/Engine.MaterialInterface,/Script/Engine.SlateTextureAtlasInterface", DisallowedClasses = "/Script/MediaAssets.MediaTexture"))
	TSoftObjectPtr<UObject> Image;

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Debug")
	bool bDeprecated{ false };

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Debug")
	bool bDisabled{ false };

public:
	UFUNCTION(BlueprintCallable, Category = "Request")
	virtual void CreateRequestsFromThis(
		UOnlineLobbySubsystem* LobbySubsystem
		, ULobbyCreateRequest*& CreateRequest
		, ULobbySearchRequest*& SearchRequest) const;

	UFUNCTION(BlueprintCallable, Category = "Request")
	virtual void CreateRequestsForMatchmaking(
		UOnlineLobbySubsystem* LobbySubsystem
		, APlayerController* HostingPlayerController
		, ULobbyCreateRequest*& CreateRequest
		, ULobbySearchRequest*& SearchRequest) const;

	UFUNCTION(BlueprintCallable, Category = "Request")
	virtual void CreateRequestsForRejoin(
		UOnlineLobbySubsystem* LobbySubsystem
		, const FString& LobbyUniqueID
		, ULobbyCreateRequest*& CreateRequest
		, ULobbySearchRequest*& SearchRequest) const;
};
