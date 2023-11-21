// Copyright 2023 RengarTheRed All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class SOLOHORROR_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	void QuitButton();
};