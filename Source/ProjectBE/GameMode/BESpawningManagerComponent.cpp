// Copyright (C) 2024 owoDra

#include "BESpawningManagerComponent.h"

#include "EngineUtils.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/GameModeBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BESpawningManagerComponent)

////////////////////////////////////////////////////////////////////////////

UBESpawningManagerComponent::UBESpawningManagerComponent(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(false);
	bAutoRegister = true;
	bAutoActivate = true;
	bWantsInitializeComponent = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}


// Initialization

void UBESpawningManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	FWorldDelegates::LevelAddedToWorld.AddUObject(this, &ThisClass::OnLevelAdded);

	auto* World{ GetWorld() };
	World->AddOnActorSpawnedHandler(FOnActorSpawned::FDelegate::CreateUObject(this, &ThisClass::HandleOnActorSpawned));

	CachePlayerStartsFromWorld(World);
}

void UBESpawningManagerComponent::OnLevelAdded(ULevel* InLevel, UWorld* InWorld)
{
	if (InWorld == GetWorld())
	{
		CachePlayerStartsFromLevel(InLevel);
	}
}

void UBESpawningManagerComponent::HandleOnActorSpawned(AActor* SpawnedActor)
{
	CachePlayerStartFromSpecificActor(SpawnedActor);
}


// Cache Player Start

void UBESpawningManagerComponent::CachePlayerStartsFromWorld(UWorld* InWorld)
{
	for (auto It{ TActorIterator<APlayerStart>(InWorld) }; It; ++It)
	{
		if (auto* PlayerStart{ *It })
		{
			CachedPlayerStarts.Add(PlayerStart);
		}
	}
}

void UBESpawningManagerComponent::CachePlayerStartsFromLevel(ULevel* InLevel)
{
	for (const auto& Actor : InLevel->Actors)
	{
		CachePlayerStartFromSpecificActor(Actor);
	}
}

void UBESpawningManagerComponent::CachePlayerStartFromSpecificActor(AActor* InActor)
{
	if (auto* PlayerStart{ Cast<APlayerStart>(InActor) })
	{
		CachedPlayerStarts.Add(PlayerStart);
	}
}


// Choose Player Start

AActor* UBESpawningManagerComponent::ChoosePlayerStart(AController* Player)
{
	if (Player)
	{
		TArray<APlayerStart*> StarterPoints;
		for (auto StartIt{ CachedPlayerStarts.CreateIterator() }; StartIt; ++StartIt)
		{
			if (auto* Start{ (*StartIt).Get() })
			{
				StarterPoints.Add(Start);
			}
			else
			{
				StartIt.RemoveCurrent();
			}
		}

		// 観戦の場合はランダムな地点を返す

		if (auto* PlayerState{ Player->GetPlayerState<APlayerState>() })
		{
			if (PlayerState->IsOnlyASpectator())
			{
				if (!StarterPoints.IsEmpty())
				{
					return StarterPoints[FMath::RandRange(0, StarterPoints.Num() - 1)];
				}

				return nullptr;
			}
		}

		// カスタム可能なロジックからスタート地点を選択する

		auto* PlayerStart{ OnChoosePlayerStart(Player, StarterPoints) };

		// カスタムロジックが実装されていない場合はランダムな位置から開始

		if (!PlayerStart)
		{
			PlayerStart = GetFirstRandomUnoccupiedPlayerStart(Player, StarterPoints);
		}

		return PlayerStart;
	}

	return nullptr;
}


// Utilities

APlayerStart* UBESpawningManagerComponent::GetFirstRandomUnoccupiedPlayerStart(AController* Controller, const TArray<APlayerStart*>& FoundStartPoints) const
{
	auto* const World{ GetWorld() };
	auto* AuthGameMode{ World ? World->GetAuthGameMode() : nullptr };
	auto* PawnClass{ AuthGameMode ? AuthGameMode->GetDefaultPawnClassForController(Controller) : nullptr };
	const auto* PawnToFit{ PawnClass ? GetDefault<APawn>(PawnClass) : nullptr };

	if (HasAuthority() && PawnToFit && Controller)
	{
		TArray<APlayerStart*> UnOccupiedStartPoints;
		TArray<APlayerStart*> OccupiedStartPoints;

		for (auto* StartPoint : FoundStartPoints)
		{
			auto State{ GetPlayerStartOccupancy(World, StartPoint, PawnToFit) };

			switch (State)
			{
			case EPlayerStartLocationOccupancy::Empty:
				UnOccupiedStartPoints.Add(StartPoint);
				break;
			case EPlayerStartLocationOccupancy::Partial:
				OccupiedStartPoints.Add(StartPoint);
				break;
			}
		}

		if (!UnOccupiedStartPoints.IsEmpty())
		{
			return UnOccupiedStartPoints[FMath::RandRange(0, UnOccupiedStartPoints.Num() - 1)];
		}
		else if (!OccupiedStartPoints.IsEmpty())
		{
			return OccupiedStartPoints[FMath::RandRange(0, OccupiedStartPoints.Num() - 1)];
		}
	}

	return nullptr;
}

EPlayerStartLocationOccupancy UBESpawningManagerComponent::GetPlayerStartOccupancy(UWorld* World, const APlayerStart* PlayerStart, const AActor* TestActor) const
{
	if (World && PlayerStart && TestActor)
	{
		auto ActorLocation{ PlayerStart->GetActorLocation() };
		const auto ActorRotation{ PlayerStart->GetActorRotation() };

		if (!World->EncroachingBlockingGeometry(TestActor, ActorLocation, ActorRotation, nullptr))
		{
			return EPlayerStartLocationOccupancy::Empty;
		}
		else if (World->FindTeleportSpot(TestActor, ActorLocation, ActorRotation))
		{
			return EPlayerStartLocationOccupancy::Partial;
		}
	}

	return EPlayerStartLocationOccupancy::Full;
}
