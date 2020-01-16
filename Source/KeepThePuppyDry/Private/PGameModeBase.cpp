// Copyright 2019, Kevin VanHorn. All rights reserved.


#include "PGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "PSwipeDelegates.h"

APGameModeBase::APGameModeBase() {
	bSpawnLoadingScreen = false;
}

void APGameModeBase::StartPlay()
{
	Super::StartPlay();

	UPSwipeDelegates::EndLoadingScreenDelegate.AddUObject(this, &APGameModeBase::EndLoadingScreen);

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
