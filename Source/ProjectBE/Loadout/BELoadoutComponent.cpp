// Copyright (C) 2024 owoDra

#include "BELoadoutComponent.h"

#include "Loadout/Type/BELoadoutRequestResolver.h"

#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"
#include "Engine/ActorChannel.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BELoadoutComponent)

//////////////////////////////////////////////////////////////////////////////////////////

UBELoadoutComponent::UBELoadoutComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}


void UBELoadoutComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	Params.Condition = COND_None;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, Loadout, Params);
}


void UBELoadoutComponent::OnRep_Loadout()
{
	HandleLoadoutChange();
}

void UBELoadoutComponent::LoadoutRequest(const FBELoadoutRequest& Request)
{
	if (Request.IsValid())
	{
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

	auto Resolver{ FBELoadoutRequestResolver(Request) };
	Resolver.ResolveRequest(Loadout);

	HandleLoadoutChange();
}

void UBELoadoutComponent::HandleLoadoutChange()
{
	RebuildIndexMap();

	OnLoadoutChange.Broadcast(this);
}

void UBELoadoutComponent::RebuildIndexMap()
{
	Loadout.IndexMap.Reset();

	for (auto It{ Loadout.Entries.CreateConstIterator() }; It; ++It)
	{
		Loadout.IndexMap.Emplace(It->GetSlotTag(), It.GetIndex());
	}
}


const UBEEquipmentItemData* UBELoadoutComponent::GetItemDataBySlot(FGameplayTag SlotTag) const
{
	if (auto* Index{ Loadout.IndexMap.Find(SlotTag) })
	{
		return Loadout.Entries[*Index].ItemData;
	}

	return nullptr;
}

const FName UBELoadoutComponent::GetSkinNameBySlot(FGameplayTag SlotTag) const
{
	if (auto* Index{ Loadout.IndexMap.Find(SlotTag) })
	{
		return Loadout.Entries[*Index].SkinName;
	}

	return FName();
}


UBELoadoutComponent* UBELoadoutComponent::FindBELoadoutComponent(const APlayerState* PlayerState)
{
	return PlayerState ? PlayerState->FindComponentByClass<UBELoadoutComponent>() : nullptr;
}
