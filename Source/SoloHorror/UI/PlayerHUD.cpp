// Copyright 2023 RengarTheRed All rights reserved.


#include "PlayerHUD.h"
#include "Kismet/GameplayStatics.h"


void UPlayerHUD::QuitButton()
{
	UGameplayStatics::OpenLevel(this, FName("MainMenu"), true);
}