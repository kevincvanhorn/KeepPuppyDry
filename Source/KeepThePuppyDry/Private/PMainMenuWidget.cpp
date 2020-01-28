// Copyright 2020, Kevin VanHorn. All rights reserved.


#include "PMainMenuWidget.h"
#include "Components/Button.h"
#include "PPlayerController.h"
#include "PSwipeDelegates.h"
#include "Components/CanvasPanel.h"
#include "PPlayerState.h"
#include "PGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "PCustomizationManager.h"
#include "Components/TextBlock.h"

void UPMainMenuWidget::NativeConstruct() {
	Super::NativeConstruct();

	Menus = { TitleScreen, GameOverScreen, TutorialScreen, CountdownScreen };
	UPSwipeDelegates::GameOverDelegate.AddUObject(this, &UPMainMenuWidget::OnGameOver);
	UPSwipeDelegates::TouchBeganDelegate.AddUObject(this, &UPMainMenuWidget::OnTouchBegin);
	UPSwipeDelegates::TouchEndedDelegate.AddUObject(this, &UPMainMenuWidget::OnTouchEnd);
	UPSwipeDelegates::PostBeginPlayDelegate.AddUObject(this, &UPMainMenuWidget::PostBeginPlay);

	bTutorialInProgress = false;
	bVolumeOn = true;

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
	// Game has already been loaded.
	PPlayerController = ControllerIn;
	PPlayerState = PPlayerStateIn;
	bSkipMenus = bSkipMenuIn;
	Menus = { TitleScreen, GameOverScreen, TutorialScreen, CountdownScreen };
	if (bSkipMenus) {
		SetScreenVisible(nullptr);
		this->OnSkipMenu();
	}
	if (PPlayerState) {
		UpdateScore(PPlayerState->GetScore());
	}
}

void UPMainMenuWidget::UpdateScore(int32 ScoreIn)
{
	if (ScoreWidget_Shop) {
		ScoreWidget_Shop->SetText(FText::AsNumber(ScoreIn));
		this->OnUpdateScore_Shop();
	}
}

void UPMainMenuWidget::SelectUmbrellaPattern(EUmbrellaPattern UmbrellaPattern)
{
	if (CustomizationManager) {
		CustomizationManager->SelectUmbrellaPattern(UmbrellaPattern);
		ShoppingCostText->SetText(FText::AsNumber(CustomizationManager->GetCost(UmbrellaPattern)));
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

void UPMainMenuWidget::PlaySound2D(USoundBase* Sound, ESoundType SoundType, float VolumeMultiplier, float PitchMultiplier, float StartTime)
{
	if (PGameInstance) {
		PGameInstance->PlaySound2D(Sound, SoundType, VolumeMultiplier, PitchMultiplier, StartTime);
	}
}

void UPMainMenuWidget::PlayPersisentSound(ESoundLabels SoundLabel, float FadeInDuration)
{
	if (PGameInstance) {
		PGameInstance->PlayPersisentSound(SoundLabel, FadeInDuration);
	}
}

void UPMainMenuWidget::StopPersistentSound(ESoundLabels SoundLabel, float FadeOutDuration)
{
	if (PGameInstance) {
		PGameInstance->StopPersistentSound(SoundLabel, FadeOutDuration);
	}
}

void UPMainMenuWidget::SetVolume(float MusicVolume, float SFXVolume)
{
	if (PGameInstance) {
		PGameInstance->SetVolume(MusicVolume, SFXVolume);
	}
}

bool UPMainMenuWidget::OnMuteButtonPressedBP()
{
	bVolumeOn = !bVolumeOn;

	if (bVolumeOn) {
		SetVolume(1.0f, 1.0f);
		if (MuteButton) {
			MuteButton->WidgetStyle.Normal.SetResourceObject(SoundOnImageObj);
			MuteButton->WidgetStyle.Pressed.SetResourceObject(SoundOnImageObj);
			MuteButton->WidgetStyle.Hovered.SetResourceObject(SoundOnImageObj);
		}
	}
	else {
		SetVolume(0.0f, 0.0f);
		if (MuteButton) {
			MuteButton->WidgetStyle.Normal.SetResourceObject(SoundOffImageObj);
			MuteButton->WidgetStyle.Pressed.SetResourceObject(SoundOffImageObj);
			MuteButton->WidgetStyle.Hovered.SetResourceObject(SoundOffImageObj);
		}
	}
	return bVolumeOn;
}

bool UPMainMenuWidget::BuyUmbrellaPattern(EUmbrellaPattern UmbrellaPattern)
{
	bool bBuySuccess = false;
	UE_LOG(LogTemp, Warning, TEXT("Message"));
	if (CustomizationManager) {
		bBuySuccess = CustomizationManager->BuyUmbrellaPattern(UmbrellaPattern);
		if (bBuySuccess && PPlayerState) {
			UpdateScore(PPlayerState->GetScore());
		}
	}

	return bBuySuccess;
}

TArray<EUmbrellaPattern> UPMainMenuWidget::GetLoadedUmbrellaPatterns()
{
	TArray<EUmbrellaPattern> Patterns;
	if (PPlayerState) {
		for (uint8 Patt : PPlayerState->GetUmbrellaPatterns()) {
			Patterns.Add((EUmbrellaPattern) Patt);
		}
	}
	return Patterns;
}
