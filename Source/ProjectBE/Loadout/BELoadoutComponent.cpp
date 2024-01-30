// Copyright (C) 2024 owoDra

#include "BELoadoutComponent.h"

#include "Item/BEEquipmentItemData.h"
#include "GameplayTag/BETags_LoadingType.h"
#include "ProjectBELogs.h"

// Game Loading Core
#include "LoadingScreenSubsystem.h"

// Engine Feature
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"
#include "Engine/ActorChannel.h"
#include "Engine/AssetManager.h"
#include "GameFeaturesSubsystemSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BELoadoutComponent)

//////////////////////////////////////////////////////////////////////////////////////////

UBELoadoutComponent::UBELoadoutComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}


// Deinitialization

void UBELoadoutComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	HandleHideLoadingLoadoutAssetScreen();

	Super::EndPlay(EndPlayReason);
}


// Replication

void UBELoadoutComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	Params.Condition = COND_None;
	Params.RepNotifyCondition = REPNOTIFY_Always;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, Loadout, Params);
}


// Loadout Request

void UBELoadoutComponent::OnRep_Loadout()
{
	// 新しいロードアウトのアイテムデータのアセットバンドルをロードする

	LoadLoadoutAssetBundle(Loadout);
}

void UBELoadoutComponent::LoadoutRequest(const FBELoadoutRequest& Request)
{
	if (Request.IsValid())
	{
		UE_LOG(LogBE_Loadout, Log, TEXT("[%s] SendLoadoutRequest: %s"), HasAuthority() ? TEXT("SERVER") : TEXT("CLIENT"), *Request.GetDebugString());

		if (HasAuthority())
		{
			ReciveLoadoutRequest(Request);
		}
		else
		{
			ServerLoadoutRequest(Request);
		}
	}
}

bool UBELoadoutComponent::ServerLoadoutRequest_Validate(const FBELoadoutRequest& Request)
{
	return true;
}

void UBELoadoutComponent::ServerLoadoutRequest_Implementation(const FBELoadoutRequest& Request)
{
	ReciveLoadoutRequest(Request);
}

void UBELoadoutComponent::ReciveLoadoutRequest(const FBELoadoutRequest& Request)
{
	check(HasAuthority());

	UE_LOG(LogBE_Loadout, Log, TEXT("[%s] ReciveLoadoutRequest: %s"), HasAuthority() ? TEXT("SERVER") : TEXT("CLIENT"), *Request.GetDebugString());

	// リクエストの ItemData を設定

	Loadout = { Request.FighterData, Request.WeaponData, Request.MainSkillData, Request.SubSkillData, Request.UltimateSkillData };
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, Loadout, this);

	// 新しいロードアウトのアイテムデータのアセットバンドルをロードする

	LoadLoadoutAssetBundle(Loadout);
}


// Loadout Asset Bundle

void UBELoadoutComponent::LoadLoadoutAssetBundle(const TArray<const UBEEquipmentItemData*>& CurrentLoadout)
{
	if (!Loadout.IsEmpty())
	{
		UE_LOG(LogBE_Loadout, Log, TEXT("[%s] StartLoadLoadoutAssetBundle"), HasAuthority() ? TEXT("SERVER") : TEXT("CLIENT"));

		// ローディング画面を表示

		HandleShowLoadingLoadoutAssetScreen();

		// 現在のロードアウト内の ItemData のプライマリーアセットIdをキャッシュする

		TSet<FPrimaryAssetId> BundleAssetList;

		for (const auto& ItemData : CurrentLoadout)
		{
			if (ensure(ItemData))
			{
				BundleAssetList.Add(ItemData->GetPrimaryAssetId());
			}
		}

		// クライアント / サーバーごとにロードするバンドル名をキャッシュする

		TArray<FName> BundlesToLoad;

		const auto OwnerNetMode{ GetOwner()->GetNetMode() };
		const auto bLoadClient{ GIsEditor || (OwnerNetMode != NM_DedicatedServer) };
		const auto bLoadServer{ GIsEditor || (OwnerNetMode != NM_Client) };

		if (bLoadClient)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateClient);
		}

		if (bLoadServer)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer);
		}

		// キャッシュしたデータをもとにバンドルのロードを開始する

		const auto BundleLoadAssetHandle
		{
			UAssetManager::Get().ChangeBundleStateForPrimaryAssets(
				BundleAssetList.Array(), BundlesToLoad, {}, false, FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority)	/// @ToDo: このまま高優先度にするかどうか検証する
		};

		auto OnAssetsLoadedDelegate{ FStreamableDelegate::CreateUObject(this, &ThisClass::HandleLoadoutAssetBundleLoaded) };

		if (!BundleLoadAssetHandle.IsValid() || BundleLoadAssetHandle->HasLoadCompleted())
		{
			FStreamableHandle::ExecuteDelegate(OnAssetsLoadedDelegate);
		}
		else
		{
			BundleLoadAssetHandle->BindCompleteDelegate(OnAssetsLoadedDelegate);

			BundleLoadAssetHandle->BindCancelDelegate(
				FStreamableDelegate::CreateLambda(
					[OnAssetsLoadedDelegate]()
					{
						OnAssetsLoadedDelegate.ExecuteIfBound();
					}
				)
			);
		}
	}
	else
	{
		UE_LOG(LogBE_Loadout, Warning, TEXT("Tried to load but no item data"));
	}
}

void UBELoadoutComponent::HandleLoadoutAssetBundleLoaded()
{
	UE_LOG(LogBE_Loadout, Log, TEXT("[%s] LoadLoadoutAssetBundleFinished"), HasAuthority() ? TEXT("SERVER") : TEXT("CLIENT"));

	// 変更を通知

	BroadcastLoadoutChange();

	// ロード画面を非表示

	HandleHideLoadingLoadoutAssetScreen();
}


// Loading Screen

void UBELoadoutComponent::HandleShowLoadingLoadoutAssetScreen()
{
	auto* PlayerController{ GetPlayerState<APlayerState>()->GetPlayerController() };

	if (PlayerController && PlayerController->IsLocalPlayerController())
	{
		if (auto* LSSubsystem{ UGameInstance::GetSubsystem<ULoadingScreenSubsystem>(GetGameInstance<UGameInstance>()) })
		{
			LSSubsystem->AddLoadingProcess(
				UBELoadoutComponent::NAME_LoadoutAssetLoading
				, TAG_LoadingType_AsyncIndicator
				, FText(NSLOCTEXT("LoadingScreen", "LoadoutAssetLoadingReason", "Loading Loadout")));
		}
	}
}

void UBELoadoutComponent::HandleHideLoadingLoadoutAssetScreen()
{
	auto* PlayerController{ GetPlayerState<APlayerState>()->GetPlayerController() };

	if (PlayerController && PlayerController->IsLocalPlayerController())
	{
		if (auto* LSSubsystem{ UGameInstance::GetSubsystem<ULoadingScreenSubsystem>(GetGameInstance<UGameInstance>()) })
		{
			LSSubsystem->RemoveLoadingProcess(UBELoadoutComponent::NAME_LoadoutAssetLoading);
		}
	}
}


// Notify Change

void UBELoadoutComponent::BroadcastLoadoutChange()
{
	OnLoadoutChange.Broadcast(this);
}


// Utilites

UBELoadoutComponent* UBELoadoutComponent::FindBELoadoutComponent(const APlayerState* PlayerState)
{
	return PlayerState ? PlayerState->FindComponentByClass<UBELoadoutComponent>() : nullptr;
}
