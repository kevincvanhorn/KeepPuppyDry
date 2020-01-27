// Copyright 2020, Kevin VanHorn. All rights reserved.


#include "PPlayerState.h"
#include "PUserWidget.h"
#include "PSaveGame.h"
#include "KeepThePuppyDry.h"
#include "PSwipeDelegates.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PGameInstance.h"

APPlayerState::APPlayerState() {
	ScoreMultiplier = 1.0f;
	Health = 1.0f;
	HPositiveRate = HNegativeRate = 1.0f;
	UnclampedHealth = 1.0f;
	bShowTutorial = true;
	bIsGameOver = false;
	//NumSessionLosses = 0;
	bShowAdOnNextRequest = false;
	TriesBetweenInterstitialAds = 1;
}

bool APPlayerState::LoadGame()
{
	if (!PGameInstance) {
		PGameInstance = Cast<UPGameInstance>(GetGameInstance());
	}

	// Retrieve and cast the USaveGame object to UMySaveGame.
	if (UPSaveGame* LoadedGame = Cast<UPSaveGame>(UGameplayStatics::LoadGameFromSlot(UPSaveGame::SaveSlotName, UPSaveGame::UserIndex)))
	{
		// The operation was successful, so LoadedGame now contains the data we saved earlier.
		PScore = LoadedGame->Currency;
		//bShowTutorial =  LoadedGame->bShowTutorial;
		
		if (GetNumSessionLosses() > 0) bShowTutorial = false;

		UE_LOG(LogTemp, Warning, TEXT("LOADED: %d"), LoadedGame->Currency);
		return true;
	}
	return false;
}

bool APPlayerState::SaveGame()
{
	return UPSaveGame::SynchronousSave(PScore, bShowTutorial);
}

bool APPlayerState::PlayerOwnsAsset(EUmbrellaPattern Pattern)
{
	return UmbrellaPatterns.Contains(Pattern);
}

bool APPlayerState::BuyAsset(EUmbrellaPattern UmbrellaPattern, int32 Cost)
{
	if (!PlayerOwnsAsset(UmbrellaPattern) && bCanAffordAsset(Cost)) {
		PScore = PScore - Cost;
		UmbrellaPatterns.Add(UmbrellaPattern);
		SaveGame();
		return true;
	}
	return false;
}

bool APPlayerState::bCanAffordAsset(int32 Cost)
{
	return PScore - Cost >= 0;
}

bool APPlayerState::bCanDisplayInterstitialAd()
{
	int32 NumSessionLosses = GetNumSessionLosses();
	if (NumSessionLosses % TriesBetweenInterstitialAds == 0 || bShowAdOnNextRequest) {
		if (UKismetSystemLibrary::IsInterstitialAdAvailable()) {
			bShowAdOnNextRequest = false;
			return true;
		}
		else {
			bShowAdOnNextRequest = true;
			return false;
		}
	}
	else {
		if (UKismetSystemLibrary::IsInterstitialAdRequested()) {
			// Continue
		}
		else {
			UKismetSystemLibrary::LoadInterstitialAd(1);
		}
	}
	return false;
}

int32 APPlayerState::ScoreFromTime(float TotalTime)
{
	return TotalTime * ScoreMultiplier;
}

float APPlayerState::GetHealth() const
{
	return Health;
}

int32 APPlayerState::GetScore() const
{
	return PScore;
}

int32 APPlayerState::GetNumSessionLosses() const
{
	if (PGameInstance) {
		return PGameInstance->GetNumSessionLosses();
	}
	return 0;
}

float APPlayerState::ChangeHealth(float Delta)
{
	return Delta >= 0 ? AddHealth(Delta) : SubtractHealth(-1*Delta);
}

// Supports adding negative
float APPlayerState::AddHealth(float DeltaTime)
{
	// UnclampedHealth can go under zero, but not gameplay relevant.
	UnclampedHealth = (UnclampedHealth + DeltaTime * HPositiveRate);
	if (UnclampedHealth >= 2) {
		UnclampedHealth  = FMath::Clamp(UnclampedHealth, 0.0f, 1.0f);
		IncrementScore();
	}
	Health = FMath::Clamp(UnclampedHealth, 0.0f, 1.0f);
	if (PUserWidget) {
		PUserWidget->SetHealthOver(FMath::Clamp(UnclampedHealth - 1.0f, 0.0f,1.0f));
	}
	return Health;
}

float APPlayerState::SubtractHealth(float DeltaTime)
{
	UnclampedHealth = FMath::Clamp(UnclampedHealth, 0.0f, 1.0f); // Reset to 1 if above when subtracting
	UnclampedHealth = (UnclampedHealth + DeltaTime * HNegativeRate * -1);
	Health = FMath::Clamp(UnclampedHealth, 0.0f, 1.0f);
	if (PUserWidget) {
		PUserWidget->SetHealthOver(0.0f);
		
		if (UnclampedHealth < 0.0f && !bIsGameOver) {
			GameOver();
			UPSwipeDelegates::GameOverDelegate.Broadcast();
		}
	}
	return Health;
}

void APPlayerState::IncrementScore()
{
	PScore++;
	if (PUserWidget) {
		PUserWidget->UpdateScore(PScore);
	}
}

void APPlayerState::SetPUserWidget(UPUserWidget* PUserWidgetIn)
{
	PUserWidget = PUserWidgetIn;
}

void APPlayerState::BeginPlay()
{
	Super::BeginPlay();
	if (!PGameInstance) {
		PGameInstance = Cast<UPGameInstance>(GetGameInstance());
	}
}

void APPlayerState::GameOver()
{
	//NumSessionLosses++;
	if (PGameInstance) {
		PGameInstance->IncrementNumSessionLosses();
	}
	if (SaveGame()) {
		bIsGameOver = true;
		UE_LOG(LogTemp, Warning, TEXT("Saved!"));
	}
}
