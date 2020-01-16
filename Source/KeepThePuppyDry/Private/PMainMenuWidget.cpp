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
	UPSwipeDelegates::TouchBeganDelegate.AddUObject(this, &UPMainMenuWidget::OnTouchBegin);
	UPSwipeDelegates::TouchEndedDelegate.AddUObject(this, &UPMainMenuWidget::OnTouchEnd);

	bTutorialInProgress = false;
}

void UPMainMenuWidget::StartGame()
{
	bool bShowTutorial = false;
	if (PPlayerController) {
		bShowTutorial = PPlayerController->StartGame();
	}
	if (bShowTutorial) {
		SetScreenVisible(TutorialScreen);
		bTutorialInProgress = true;
	}
	else {
		SetScreenVisible(nullptr); // Hide all screens
	}
}

void UPMainMenuWidget::EndTutorial()
{
	bTutorialInProgress = false;
	SetScreenVisible(nullptr);
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

void UPMainMenuWidget::EndLoadingScreen()
{
	UPSwipeDelegates::EndLoadingScreenDelegate.Broadcast();
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

void UPMainMenuWidget::OnTouchBegin()
{
	this->OnBPTouchBegin();

	if (bTutorialInProgress) {
		EndTutorial();
	}
}

void UPMainMenuWidget::OnTouchEnd()
{
	this->OnBPTouchEnd();
}
