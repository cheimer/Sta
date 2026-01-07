// Sta Project - Multiplay RTS with GAS


#include "StaGameModeBase.h"

#include "Controller/StaPlayerController.h"
#include "Framework/GameState/StaGameState.h"
#include "Player/CommandPawn.h"
#include "Framework/PlayerState/StaPlayerState.h"
#include "UI/StaHUD.h"

AStaGameModeBase::AStaGameModeBase()
{
	DefaultPawnClass = ACommandPawn::StaticClass();
	HUDClass = AStaHUD::StaticClass();
	PlayerControllerClass = AStaPlayerController::StaticClass();
	GameStateClass = AStaGameState::StaticClass();
	PlayerStateClass = AStaPlayerState::StaticClass();
	
}
