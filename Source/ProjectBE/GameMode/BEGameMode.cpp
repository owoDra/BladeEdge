// Copyright (C) 2024 owoDra

#include "BEGameMode.h"

#include "Character/BECharacter.h"
#include "Character/BESpectatorPawn.h"
#include "Player/BEPlayerController.h"
#include "Player/BEPlayerState.h"
#include "GameMode/BEGameState.h"
#include "UI/BEHUD.h"
#include "System/BEGameSession.h"
#include "ProjectBELogs.h"

#include "InitState/InitStateTags.h"

#include "Components/GameFrameworkInitStateInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BEGameMode)


ABEGameMode::ABEGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultPawnClass = ABECharacter::StaticClass();
	PlayerControllerClass = ABEPlayerController::StaticClass();
	PlayerStateClass = ABEPlayerState::StaticClass();
	GameStateClass = ABEGameState::StaticClass();
	HUDClass = ABEHUD::StaticClass();
	GameSessionClass = AGameSession::StaticClass();
	SpectatorClass = ABESpectatorPawn::StaticClass();
	ReplaySpectatorPlayerControllerClass = ABEPlayerController::StaticClass();
}
