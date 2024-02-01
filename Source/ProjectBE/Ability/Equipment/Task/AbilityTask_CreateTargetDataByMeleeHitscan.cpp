// Copyright (C) 2024 owoDra

#include "AbilityTask_CreateTargetDataByMeleeHitscan.h"

#include "GAEGameplayAbility.h"

#include "GameFramework/Controller.h"
#include "DrawDebugHelpers.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AbilityTask_CreateTargetDataByMeleeHitscan)


/////////////////////////////////////////////////////////////////////////////////////

namespace BEConsolVariables
{
	static float DrawMeleeHitscanTracesDuration = 0.0f;
	static FAutoConsoleVariableRef CVarDrawBulletTraceDuraton(
		TEXT("BE.DrawMeleeHitscanTracesDuration"),
		DrawMeleeHitscanTracesDuration,
		TEXT("Should we do debug drawing for melee hitscan traces (if above zero, sets how long (in seconds))"),
		ECVF_Default);
}

/////////////////////////////////////////////////////////////////////////////////////


UAbilityTask_CreateTargetDataByMeleeHitscan::UAbilityTask_CreateTargetDataByMeleeHitscan(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


UAbilityTask_CreateTargetDataByMeleeHitscan* UAbilityTask_CreateTargetDataByMeleeHitscan::CreateCreateTargetDataByMeleeHitscan(
	UGameplayAbility* OwningAbility
	, const TArray<TEnumAsByte<EObjectTypeQuery>>& InObjectTypes
	, const TArray<AActor*>& InActorsToIgnore
	, bool bInIgnoreAvatarAndChildren
	, ECollisionChannel InCollisionChannel
	, float InDuration
	, float InRadius
	, float InDistance)
{
	auto* Task{ NewAbilityTask<UAbilityTask_CreateTargetDataByMeleeHitscan>(OwningAbility) };
	Task->ObjectTypes = InObjectTypes;
	Task->ActorsToIgnore = InActorsToIgnore;
	Task->bIgnoreAvatarAndChildren = bInIgnoreAvatarAndChildren;
	Task->CollisionChannel = InCollisionChannel;
	Task->Duration = InDuration;
	Task->Radius = InRadius;
	Task->Distance = InDistance;

	return Task;
}

void UAbilityTask_CreateTargetDataByMeleeHitscan::Activate()
{
	StoreAvatarToIgnoreList();

	if (!TryCreateTargetData())
	{
		if (!RegisterTimerEvent())
		{
			HandleTargetDataNotCreated();
		}
	}
}

void UAbilityTask_CreateTargetDataByMeleeHitscan::OnDestroy(bool bInOwnerFinished)
{
	UnregisterTimerEvent();

	Super::OnDestroy(bInOwnerFinished);
}


bool UAbilityTask_CreateTargetDataByMeleeHitscan::TryCreateTargetData()
{
	// Determine whether LineTrace or CylinderTrace

	const auto bUseCylinderTrace{ Radius > 0.0f };

	// Perform tracing

	FHitResult EnemyTraceHitResult;
	const auto bFoundEnemy{ bUseCylinderTrace ? PerformCylinderTraceTrace(EnemyTraceHitResult) : PerformLineTrace(EnemyTraceHitResult) };

	// Returns false if not found

	if (!bFoundEnemy)
	{
		return false;
	}

	// Create new target data from HitResult if HitResult is valid

	if (PerformValidationLineTrace(EnemyTraceHitResult))
	{
		auto* NewTargetData{ new FGameplayAbilityTargetData_SingleTargetHit(EnemyTraceHitResult) };
		auto NewTargetDataHandle{ FGameplayAbilityTargetDataHandle(NewTargetData) };

		HandleTargetDataCreated(NewTargetDataHandle);

		return true;
	}

	return false;
}

void UAbilityTask_CreateTargetDataByMeleeHitscan::StoreAvatarToIgnoreList()
{
	if (Ability && bIgnoreAvatarAndChildren)
	{
		if (auto* Avatar{ Ability->GetAvatarActorFromActorInfo() })
		{
			ActorsToIgnore.Add(Avatar);

			TArray<AActor*> ChildActors;
			Avatar->GetAllChildActors(ChildActors);

			ActorsToIgnore.Append(ChildActors);
		}
	}
}


#pragma region Repeat Process

bool UAbilityTask_CreateTargetDataByMeleeHitscan::RegisterTimerEvent()
{
	if (Duration > 0.0f)
	{
		SetTimer(/*bInitalSet = */ true);

		return true;
	}
	else
	{
		return false;
	}
}

void UAbilityTask_CreateTargetDataByMeleeHitscan::UnregisterTimerEvent()
{
	if (auto* World{ GetWorld() })
	{
		World->GetTimerManager().ClearTimer(RepeatTimerHandle);
	}
}

void UAbilityTask_CreateTargetDataByMeleeHitscan::PerformTick()
{
	// Skip processing once TargetData is created.

	if (TryCreateTargetData())
	{
		return;
	}

	// If TargetData is not created, check if it is possible to continue.

	auto* World{ GetWorld() };
	const auto TimeElapse{ World ? (World->GetTimeSeconds() - StartTime) : Duration };

	if (TimeElapse >= Duration)
	{
		HandleTargetDataNotCreated();
	}
	else
	{
		SetTimer();
	}
}

void UAbilityTask_CreateTargetDataByMeleeHitscan::SetTimer(bool bInitalSet)
{
	auto* World{ GetWorld() };

	if (bInitalSet)
	{
		StartTime = World->GetTimeSeconds();
	}

	RepeatTimerHandle = World->GetTimerManager().SetTimerForNextTick(this, &ThisClass::PerformTick);
}

#pragma endregion


#pragma region Trace

bool UAbilityTask_CreateTargetDataByMeleeHitscan::PerformLineTrace(FHitResult& OutResult)
{
	FVector Start;
	FVector End;
	CalcTraceStartEnd(Start, End);

	auto ObjectQueryParams{ FCollisionObjectQueryParams(ObjectTypes) };
	auto CollisionQueryParams{ FCollisionQueryParams(SCENE_QUERY_STAT(MeleeTrace), /*bTraceComplex=*/ true) };
	CollisionQueryParams.AddIgnoredActors(ActorsToIgnore);

	const auto bResult{ GetWorld()->LineTraceSingleByObjectType(OutResult, Start, End, ObjectQueryParams, CollisionQueryParams) };

#if ENABLE_DRAW_DEBUG
	if (BEConsolVariables::DrawMeleeHitscanTracesDuration > 0.0f)
	{
		static float DebugThickness = 1.0f;
		DrawDebugLine(GetWorld(), OutResult.TraceStart, bResult ? OutResult.ImpactPoint : OutResult.TraceEnd, FColor::Red, false, BEConsolVariables::DrawMeleeHitscanTracesDuration, 0, DebugThickness);
		if (bResult) DrawDebugLine(GetWorld(), OutResult.ImpactPoint, OutResult.TraceEnd, FColor::Green, false, BEConsolVariables::DrawMeleeHitscanTracesDuration, 0, DebugThickness);
	}
#endif

	return bResult;
}

bool UAbilityTask_CreateTargetDataByMeleeHitscan::PerformCylinderTraceTrace(FHitResult& OutResult)
{
	FVector Start;
	FVector End;
	CalcTraceStartEnd(Start, End);

	auto ObjectQueryParams{ FCollisionObjectQueryParams(ObjectTypes) };
	auto CollisionQueryParams{ FCollisionQueryParams(SCENE_QUERY_STAT(MeleeTrace), /*bTraceComplex=*/ true) };
	CollisionQueryParams.AddIgnoredActors(ActorsToIgnore);

	const auto bResult{ GetWorld()->SweepSingleByObjectType(OutResult, Start, End, FQuat::Identity, ObjectQueryParams, FCollisionShape::MakeSphere(Radius), CollisionQueryParams) };

#if ENABLE_DRAW_DEBUG
	if (BEConsolVariables::DrawMeleeHitscanTracesDuration > 0.0f)
	{
		static float DebugThickness = 1.0f;
		DrawDebugCylinder(GetWorld(), OutResult.TraceStart, bResult ? OutResult.ImpactPoint : OutResult.TraceEnd, Radius, 16, FColor::Red, false, BEConsolVariables::DrawMeleeHitscanTracesDuration, 0, DebugThickness);
		if (bResult) DrawDebugCylinder(GetWorld(), OutResult.ImpactPoint, OutResult.TraceEnd, Radius, 16, FColor::Green, false, BEConsolVariables::DrawMeleeHitscanTracesDuration, 0, DebugThickness);
	}
#endif
	
	return bResult;
}

bool UAbilityTask_CreateTargetDataByMeleeHitscan::PerformValidationLineTrace(const FHitResult& InHitResult)
{
	auto Start{ InHitResult.TraceStart };
	auto End{ InHitResult.bBlockingHit ? InHitResult.ImpactPoint : InHitResult.TraceEnd };

	auto CollisionQueryParams{ FCollisionQueryParams(SCENE_QUERY_STAT(MeleeTrace), /*bTraceComplex=*/ true, /*IgnoreActor=*/ InHitResult.GetActor()) };
	CollisionQueryParams.AddIgnoredActors(ActorsToIgnore);

	FHitResult DummyResult;
	const auto bResult{ !GetWorld()->LineTraceSingleByChannel(DummyResult, Start, End, CollisionChannel, CollisionQueryParams) };

#if ENABLE_DRAW_DEBUG
	if (BEConsolVariables::DrawMeleeHitscanTracesDuration > 0.0f)
	{
		static float DebugThickness = 1.0f;
		DrawDebugLine(GetWorld(), DummyResult.TraceStart, bResult ? DummyResult.ImpactPoint : DummyResult.TraceEnd, FColor::Yellow, false, BEConsolVariables::DrawMeleeHitscanTracesDuration, 0, DebugThickness);
		if (bResult) DrawDebugLine(GetWorld(), DummyResult.ImpactPoint, DummyResult.TraceEnd, FColor::Cyan, false, BEConsolVariables::DrawMeleeHitscanTracesDuration, 0, DebugThickness);
	}
#endif

	return bResult;
}

void UAbilityTask_CreateTargetDataByMeleeHitscan::CalcTraceStartEnd(FVector& OutStart, FVector& OutEnd) const
{
	if (auto* GAEAbility{ Cast<UGAEGameplayAbility>(Ability) })
	{
		if (auto* Controller{ GAEAbility->GetController() })
		{
			FRotator Rot;

			Controller->GetPlayerViewPoint(OutStart, Rot);

			OutEnd = OutStart + (Rot.Vector() * Distance);

			return;
		}
	}

	if (Ability)
	{
		if (auto* Controller{ Ability->GetCurrentActorInfo() ? Ability->GetCurrentActorInfo()->PlayerController.Get() : nullptr})
		{
			FRotator Rot;

			Controller->GetPlayerViewPoint(OutStart, Rot);

			OutEnd = OutStart + (Rot.Vector() * Distance);

			return;
		}
	}
}

#pragma endregion
