// Copyright (C) 2024 owoDra

#pragma once

#include "BECharacter.h"

#include "BEPlayerCharacter.generated.h"

class UBEViewerComponent;
class USkeletalMeshComponent;


/**
 * このプロジェクトにおける Player Character のベースクラス
 */
UCLASS(Abstract, Blueprintable)
class PROJECTBE_API ABEPlayerCharacter : public ABECharacter
{
	GENERATED_BODY()
public:
	explicit ABEPlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//
	// このキャラクターの視点を管理するコンポーネント
	//
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Components")
	TObjectPtr<UBEViewerComponent> ViewerComponent;

	//
	// このキャラクターのFPP視点の体を表現するコンポーネント
	//
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> FPPMesh;

public:
	virtual void NotifyControllerChanged() override;

public:
	TArray<USkeletalMeshComponent*> GetMeshes_Implementation() const override;
	USkeletalMeshComponent* GetMainMesh_Implementation() const override;
	USkeletalMeshComponent* GetMeshByTag_Implementation(FGameplayTag Tag) const override;

};
