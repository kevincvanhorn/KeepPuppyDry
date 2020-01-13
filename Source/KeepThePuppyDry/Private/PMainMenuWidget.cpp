// Copyright 2020, Kevin VanHorn. All rights reserved.


#include "PMainMenuWidget.h"
#include "Components/Button.h"
#include "PPlayerController.h"

void UPMainMenuWidget::StartGame()
{
	if (PPlayerController) {
		PPlayerController->StartGame();
	}
}

void UPMainMenuWidget::Initialize(APPlayerController* ControllerIn)
{
	PPlayerController = ControllerIn;
}
