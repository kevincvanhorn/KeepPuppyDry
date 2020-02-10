// Copyright 2020, Kevin VanHorn. All rights reserved.


#include "PPlayerState.h"
#include "PUserWidget.h"
#include "PSaveGame.h"
#include "KeepThePuppyDry.h"
#include "PSwipeDelegates.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PGameInstance.h"
#include "PSwipeDelegates.h"

#define IOS

#ifdef IOS
#include "EasyAdsLibrary.h"
#include "ShowInterstitialProxy.h"
#endif

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
	LastSelected_UmbrellaPattern = -1;
	SessionStartScore = 0;
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
		UmbrellaPatterns.Empty();
		UmbrellaPatterns = LoadedGame->UPatterns;
		LastSelected_UmbrellaPattern = LoadedGame->LS_UmbrellaPattern;
		
		if (GetNumSessionLosses() > 0) bShowTutorial = false;

		UE_LOG(LogTemp, Warning, TEXT("LOADED: %d"), LoadedGame->Currency);
		return true;
	}
	return false;
}

bool APPlayerState::SaveGame()
{
	return UPSaveGame::SynchronousSave(PScore, bShowTutorial, UmbrellaPatterns, LastSelected_UmbrellaPattern);
}

bool APPlayerState::PlayerOwnsAsset(EUmbrellaPattern Pattern)
{
	return UmbrellaPatterns.Contains((uint8)Pattern);
}

bool APPlayerState::BuyAsset(EUmbrellaPattern UmbrellaPattern, int32 Cost)
{
	if (!PlayerOwnsAsset(UmbrellaPattern) && bCanAffordAsset(Cost)) {
		PScore = PScore - Cost;
		UmbrellaPatterns.Add((uint8)UmbrellaPattern);
		SaveGame();
		return true;
	}
	return false;
}

bool APPlayerState::bCanAffordAsset(int32 Cost)
{
	return PScore - Cost >= 0;
}

void APPlayerState::SetLastSelected(EUmbrellaPattern UmbrellaPattern)
{
	LastSelected_UmbrellaPattern = (int32)UmbrellaPattern;
}

void APPlayerState::OnGameStart()
{
	SessionStartScore = PScore;
}

void APPlayerState::ResetLocalGameSave()
{
	PScore = 0;
	bShowTutorial =  true;
	UmbrellaPatterns.Empty();
	SaveGame();
}

void APPlayerState::IncreaseScoreRate(float PosRate, float NegRate)
{
	HPositiveRate = PosRate;
	HNegativeRate = NegRate;
}

bool APPlayerState::TryDisplayInterstitialAd()
{
	int32 NumSessionLosses = GetNumSessionLosses();
	if ((NumSessionLosses % TriesBetweenInterstitialAds == 0 || bShowAdOnNextRequest) && GetSessionScore() > 2) {
#ifndef IOS
		if (UKismetSystemLibrary::IsInterstitialAdAvailable()) {
			bShowAdOnNextRequest = false;
			UKismetSystemLibrary::ShowInterstitialAd(); // Show Ad
			return true;
		}
		else {
			bShowAdOnNextRequest = true;
			return false;
		}
#endif
#ifdef IOS
		//if (PLATFORM == "IOS") {
		if (UEasyAdsLibrary::IsInterstitialReady()) {
			//UShowInterstitialProxy::ShowInterstitial(); // Show Ad // MOVED TO BLUEPRINT
			bShowAdOnNextRequest = false;
			return true;
		}
		else {
			bShowAdOnNextRequest = true;
			return false;
		}
		//}
#endif
	}
	else {
#ifndef IOS
		if (UKismetSystemLibrary::IsInterstitialAdRequested()) {
			// Continue
		}
		else {
			UKismetSystemLibrary::LoadInterstitialAd(1);
		}
#endif
#ifdef IOS
		if (UEasyAdsLibrary::IsInterstitialReady()) {
			// Continue
		}
#endif
	}
	return false;
}

void APPlayerState::ShowAdBanner()
{
#ifndef IOS
	//if (PLATFORM == "Android") {
		UKismetSystemLibrary::ShowAdBanner(0, true); // Show ad
	//}
#endif
#ifdef IOS
		UEasyAdsLibrary::ShowBanner(true);
#endif
}

void APPlayerState::HideAdBanner()
{
#ifndef IOS
	//if (PLATFORM == "Android") {
		UKismetSystemLibrary::HideAdBanner(); // Show ad
	//}
#endif
#ifdef IOS
		UEasyAdsLibrary::HideBanner();
#endif
}

int32 APPlayerState::GetSessionScore()
{
	return PScore - SessionStartScore;
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

void APPlayerState::SetScore(int32 ScoreIn)
{
	PScore = ScoreIn;
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
	PLATFORM = UGameplayStatics::GetPlatformName();

	UPSwipeDelegates::GameStartDelegate.AddUObject(this, &APPlayerState::OnGameStart);
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

	HideAdBanner();
}
