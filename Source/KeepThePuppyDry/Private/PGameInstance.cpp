// Copyright 2019, Kevin VanHorn. All rights reserved.


#include "PGameInstance.h"
#include "MoviePlayer.h"
#include "UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

UPGameInstance::UPGameInstance() {
	bSkipToGameplay = false;
	NumSessionLosses = 0;
	GlobalMusicVolume = 1.0f;
	GlobalSFXVolume = 1.0f;
}

void UPGameInstance::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UPGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UPGameInstance::EndLoadingScreen);
}

void UPGameInstance::BeginLoadingScreen(const FString& InMapName)
{
	return; // removed because animation was not valid pre-loading.
	if (!IsRunningDedicatedServer())
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.MinimumLoadingScreenDisplayTime = MinimumLoadingScreenDisplayTime;
		if (WidgetLoadingScreenClass) {
			LoadingScreenWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetLoadingScreenClass);
			if (LoadingScreenWidget) {
				LoadingScreen.WidgetLoadingScreen = LoadingScreenWidget->TakeWidget();
			}
		}
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
		LoadingScreen.bMoviesAreSkippable = false;
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
}

void UPGameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{

}

void UPGameInstance::SetVolume(float MusicVolume, float SFXVolume)
{
	GlobalSFXVolume = SFXVolume;
	GlobalMusicVolume = MusicVolume;

	for(FPersistentSound FSound : PersistentSounds){
		if (FSound.Sound) {
			if (FSound.SoundType == ESoundType::E_Music && MusicVolume <= .01f) {
				if (MusicVolume <= .01f) {
					StopPersistentSound(FSound.SoundLabel, 0.2f);
				}
			}
			else if (FSound.SoundType ==  ESoundType::E_SFX && SFXVolume <= .01f) {
				StopPersistentSound(FSound.SoundLabel, 0.2f);
			}
		}
	}
}

void UPGameInstance::PlaySound2D(USoundBase* Sound, ESoundType SoundType, float VolumeMultiplier,
	float PitchMultiplier, float StartTime) {
	float LocalMultiplier = 1.0f;
	if (SoundType == ESoundType::E_Music) {
		LocalMultiplier = GlobalMusicVolume;
	}
	else if (SoundType == ESoundType::E_SFX) {
		LocalMultiplier = GlobalSFXVolume;
	}

	UGameplayStatics::PlaySound2D(this, Sound, VolumeMultiplier * LocalMultiplier, PitchMultiplier, StartTime, nullptr);
}

void UPGameInstance::PlayPersisentSound(ESoundLabels SoundLabel, float FadeInDuration)
{
	uint8 idx = (uint8)SoundLabel;
	if (idx < PersistentSounds.Num()) {
		FPersistentSound Details = PersistentSounds[idx];
		float LocalMultiplier = 1.0f;
		UAudioComponent* Audio;

		if (Details.SoundType == ESoundType::E_Music) {
			LocalMultiplier = GlobalMusicVolume;
		}
		else if (Details.SoundType == ESoundType::E_SFX) {
			LocalMultiplier = GlobalSFXVolume;
		}

		if (PersistentSoundMap.Contains(SoundLabel)) {
			Audio = *PersistentSoundMap.Find(SoundLabel);
		}
		else {
			Audio = UGameplayStatics::SpawnSound2D(this, Details.Sound, Details.VolumeMultiplier * LocalMultiplier, Details.PitchMultiplier, Details.StartTime, nullptr, true, false);
			PersistentSoundMap.Add(SoundLabel, Audio);
		}

		if (Audio) {
			Audio->FadeIn(FadeInDuration, Details.VolumeMultiplier * LocalMultiplier, Details.StartTime, EAudioFaderCurve::Linear);
		}
	}
}

void UPGameInstance::StopPersistentSound(ESoundLabels SoundLabel, float FadeOutDuration)
{
	uint8 idx = (uint8)SoundLabel;
	if (idx < PersistentSounds.Num()) {
		UAudioComponent* Audio;

		if (PersistentSoundMap.Contains(SoundLabel)) {
			Audio = *PersistentSoundMap.Find(SoundLabel);
			Audio->FadeOut(FadeOutDuration, 0.0f, EAudioFaderCurve::Linear);
		}
	}
}
