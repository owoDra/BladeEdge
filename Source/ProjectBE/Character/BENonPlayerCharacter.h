// Copyright (C) 2024 owoDra

#pragma once

#include "BECharacter.h"

#include "BENonPlayerCharacter.generated.h"


/** 
 * このプロジェクトにおける NPC Character のベースクラス
 */
UCLASS(Abstract, Blueprintable)
class PROJECTBE_API ABENonPlayerCharacter : public ABECharacter
{
	GENERATED_BODY()
public:
	explicit ABENonPlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	TArray<USkeletalMeshComponent*> GetMeshes_Implementation() const override;
	USkeletalMeshComponent* GetMainMesh_Implementation() const override;
	USkeletalMeshComponent* GetMeshByTag_Implementation(FGameplayTag Tag) const override;

};
