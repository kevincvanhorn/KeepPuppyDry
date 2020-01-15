// Copyright 2019, Kevin VanHorn. All rights reserved.


#include "PGameInstance.h"
#include "MoviePlayer.h"
#include "UserWidget.h"

UPGameInstance::UPGameInstance() {
}

void UPGameInstance::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UPGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UPGameInstance::EndLoadingScreen);
}

void UPGameInstance::BeginLoadingScreen(const FString& InMapName)
{
	if (!IsRunningDedicatedServer())
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.MinimumLoadingScreenDisplayTime = MinimumLoadingScreenDisplayTime;
		if (WidgetLoadingScreen) {
			LoadingScreen.WidgetLoadingScreen = WidgetLoadingScreen->TakeWidget();
		}
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;

		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
}

void UPGameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{

}