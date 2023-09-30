// Fill out your copyright notice in the Description page of Project Settings.


#include "HorrorGameMode.h"
#include "SoloHorror/Player/PlayerCharacter.h"
#include "SoloHorror/Player/HorrorPlayerController.h"

AHorrorGameMode::AHorrorGameMode()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
	PlayerControllerClass = AHorrorPlayerController::StaticClass();
}