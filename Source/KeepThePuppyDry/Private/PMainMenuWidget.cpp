// Copyright 2020, Kevin VanHorn. All rights reserved.


#include "PMainMenuWidget.h"
#include "Components/Button.h"
#include "PPlayerController.h"
#include "PSwipeDelegates.h"
#include "Components/CanvasPanel.h"
#include "PPlayerState.h"
#include "PGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UPMainMenuWidget::NativeConstruct() {
	Super::NativeConstruct();

	Menus = { TitleScreen, GameOverScreen, TutorialScreen, CountdownScreen };
	UPSwipeDelegates::GameOverDelegate.AddUObject(this, &UPMainMenuWidget::OnGameOver);
	UPSwipeDelegates::TouchBeganDelegate.AddUObject(this, &UPMainMenuWidget::OnTouchBegin);
	UPSwipeDelegates::TouchEndedDelegate.AddUObject(this, &UPMainMenuWidget::OnTouchEnd);
	UPSwipeDelegates::PostBeginPlayDelegate.AddUObject(this, &UPMainMenuWidget::PostBeginPlay);

	bTutorialInProgress = false;

	bSkipMenus = false;
	PGameInstance = (UPGameInstance*)UGameplayStatics::GetGameInstance(GetWorld());
	if (PGameInstance) {
		bSkipMenus = PGameInstance->ShouldSkipToGameplay();
	}

	if (bSkipMenus) {
		SetScreenVisible(CountdownScreen);
	}
	else {
		SetScreenVisible(TitleScreen); // Hide all screens
	}
}

void UPMainMenuWidget::StartGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false); // UnPause game.

	bool bSkipToGameplay = false;
	PGameInstance = (UPGameInstance*)UGameplayStatics::GetGameInstance(GetWorld());
	if (PGameInstance) {
		bSkipToGameplay = PGameInstance->ShouldSkipToGameplay();
	}

	bool bShowTutorial = false;
	if (PPlayerController) {
		bShowTutorial = PPlayerController->StartGame(bSkipToGameplay) && !bSkipToGameplay;
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

void UPMainMenuWidget::PInitialize(APPlayerController* ControllerIn, APPlayerState* PPlayerStateIn, bool bSkipMenuIn)
{
	PPlayerController = ControllerIn;
	PPlayerState = PPlayerStateIn;
	bSkipMenus = bSkipMenuIn;
	Menus = { TitleScreen, GameOverScreen, TutorialScreen, CountdownScreen };
	if (bSkipMenus) {
		SetScreenVisible(nullptr);
		//UGameplayStatics::SetGamePaused(GetWorld(), true);
		this->OnSkipMenu();
	}
}

void UPMainMenuWidget::OnGameOver()
{
	if (PGameInstance) {
		PGameInstance->SetSkipToGameplay(true);
	}

	if (GameOverScreen) {
		SetScreenVisible(GameOverScreen);
	}
	this->OnGameOverEventBP();

}

void UPMainMenuWidget::EndLoadingScreen()
{
	UPSwipeDelegates::EndLoadingScreenDelegate.Broadcast();
}

bool UPMainMenuWidget::bCanDisplayInterstitialAd()
{
	if (PPlayerState) {
		return PPlayerState->bCanDisplayInterstitialAd();
	}

	return true;
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

void UPMainMenuWidget::PostBeginPlay()
{
	this->OnPostBeginPlay();
}

void UPMainMenuWidget::PRestartLevel(bool bShowTitleScreen)
{
	if (PGameInstance) {
		PGameInstance->SetSkipToGameplay(!bShowTitleScreen);
	}
	if (PPlayerController) {
		PPlayerController->RestartLevel();
	}
}
