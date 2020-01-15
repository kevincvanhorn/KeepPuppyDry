// Copyright 2020, Kevin VanHorn. All rights reserved.


#include "PMainMenuWidget.h"
#include "Components/Button.h"
#include "PPlayerController.h"
#include "PSwipeDelegates.h"
#include "Components/CanvasPanel.h"

void UPMainMenuWidget::NativeConstruct() {
	Super::NativeConstruct();

	Menus = { TitleScreen, GameOverScreen, TutorialScreen};
	SetScreenVisible(TitleScreen); // Hide all screens
	UPSwipeDelegates::GameOverDelegate.AddUObject(this, &UPMainMenuWidget::OnGameOver);
}

void UPMainMenuWidget::StartGame()
{
	bool bShowTutorial = false;
	if (PPlayerController) {
		bShowTutorial = PPlayerController->StartGame();
	}
	if (bShowTutorial) {
		SetScreenVisible(TutorialScreen);
	}
	else {
		SetScreenVisible(nullptr); // Hide all screens
	}
}

void UPMainMenuWidget::EndTutorial()
{
	if (PPlayerController) {
		PPlayerController->EndTutorial();
	}
}

void UPMainMenuWidget::PInitialize(APPlayerController* ControllerIn)
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
