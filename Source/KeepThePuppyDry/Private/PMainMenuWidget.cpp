// Copyright 2020, Kevin VanHorn. All rights reserved.


#include "PMainMenuWidget.h"
#include "Components/Button.h"
#include "PPlayerController.h"
#include "PSwipeDelegates.h"
#include "Components/CanvasPanel.h"

void UPMainMenuWidget::NativeConstruct() {
	Super::NativeConstruct();

	Menus = { TitleScreen, GameOverScreen};
	SetScreenVisible(TitleScreen); // Hide all screens
	UPSwipeDelegates::GameOverDelegate.AddUObject(this, &UPMainMenuWidget::OnGameOver);
}

void UPMainMenuWidget::StartGame()
{
	SetScreenVisible(nullptr); // Hide all screens
	if (PPlayerController) {
		PPlayerController->StartGame();
	}
}

void UPMainMenuWidget::Initialize(APPlayerController* ControllerIn)
{
	PPlayerController = ControllerIn;
}

void UPMainMenuWidget::OnGameOver()
{
	if (GameOverScreen) {
		SetScreenVisible(GameOverScreen);
	}
}

void UPMainMenuWidget::SetScreenVisible(UCanvasPanel* ScreenIn)
{
	for (UCanvasPanel* Screen : Menus) {
		if (Screen != nullptr) {
			if (Screen == ScreenIn) { Screen->SetVisibility(ESlateVisibility::Visible); }
			else {
				Screen->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}
