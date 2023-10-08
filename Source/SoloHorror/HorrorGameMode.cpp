// Copyright 2023 RengarTheRed All rights reserved.


#include "HorrorGameMode.h"
#include "SoloHorror/Player/PlayerCharacter.h"
#include "SoloHorror/Player/HorrorPlayerController.h"

AHorrorGameMode::AHorrorGameMode()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
	PlayerControllerClass = AHorrorPlayerController::StaticClass();
}