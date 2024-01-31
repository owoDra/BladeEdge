// Copyright (C) 2024 owoDra

#pragma once

#include "Recipe/CharacterRecipe.h"

#include "BECharacterRecipe_Loadout.generated.h"

class UEquipmentSet;
class UBELoadoutComponent;


/**
 * プレイヤーのロードアウトを監視してロードアウトに応じて装備を更新するキャラクターレシピ
 */
UCLASS()
class UBECharacterRecipe_Loadout final : public UCharacterRecipe
{
	GENERATED_BODY()
public:
	UBECharacterRecipe_Loadout();

protected:
	virtual void StartSetup_Implementation(const FCharacterRecipePawnInfo& Info) override;
	virtual void OnDestroy_Implementation() override;

	void ListenLoadoutChange();
	void UnlistenLoadoutChange();
	void HandleLoadoutChange(const UBELoadoutComponent* LoadoutComponent);

};
