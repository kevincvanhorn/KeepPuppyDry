// Copyright 2019, Kevin VanHorn. All rights reserved.


#include "PGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "PSwipeDelegates.h"
#include "PGameInstance.h"

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
