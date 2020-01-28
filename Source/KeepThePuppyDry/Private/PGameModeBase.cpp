// Copyright 2019, Kevin VanHorn. All rights reserved.


#include "PGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "PSwipeDelegates.h"
#include "PGameInstance.h"
#include "PPlayerState.h"
#include "PPlayer.h"
#include "Kismet/GameplayStatics.h"

APGameModeBase::APGameModeBase() {
	bSpawnLoadingScreen = false;
}

void APGameModeBase::StartPlay()
{
	Super::StartPlay();

	UPSwipeDelegates::EndLoadingScreenDelegate.AddUObject(this, &APGameModeBase::EndLoadingScreen);

	UPGameInstance* PGameInstance = (UPGameInstance*)GetGameInstance();
	if (PGameInstance) {
		bSpawnLoadingScreen = !PGameInstance->ShouldSkipToGameplay();
	}

	if (bSpawnLoadingScreen) {
		if (PLoadingScreenClass) {
			PLoadingScreen = CreateWidget<UUserWidget>(GetWorld(), PLoadingScreenClass);
			if (PLoadingScreen) {
				PLoadingScreen->AddToViewport();
			}
		}
	}
}

void APGameModeBase::EndLoadingScreen()
{
	if (PLoadingScreen) {
		PLoadingScreen->RemoveFromViewport();
	}
}

void APGameModeBase::execSetScore(int32 ScoreIn)
{
	APPlayer* PPlayer = (APPlayer*)UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PPlayer) {
		APPlayerState* PPlayerState = (APPlayerState*)PPlayer->GetPlayerState();
		if (PPlayerState) {
			PPlayerState->SetScore(ScoreIn);
		}
	}
}

void APGameModeBase::execResetLocalGameSave()
{
	APPlayer* PPlayer = (APPlayer*)UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PPlayer) {
		APPlayerState* PPlayerState = (APPlayerState*)PPlayer->GetPlayerState();
		if (PPlayerState) {
			PPlayerState->ResetLocalGameSave();
		}
	}
}
