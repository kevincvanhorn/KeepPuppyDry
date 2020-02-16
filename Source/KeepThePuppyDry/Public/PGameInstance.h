// Copyright 2019, Kevin VanHorn. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PGameInstance.generated.h"


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ESoundType : uint8
{
	E_Music 		UMETA(DisplayName = "Music"),
	E_SFX 	UMETA(DisplayName = "Sound"),
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ESoundLabels : uint8
{
	E_TitleMusic 		UMETA(DisplayName = "Title Music"),
	E_Rainfall 			UMETA(DisplayName = "Rainfall"),
	E_GameMusic 		UMETA(DisplayName = "Game Music"),

	E_Last 				UMETA(DisplayName = "Last")
};

USTRUCT(BlueprintType)
struct FPersistentSound
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		USoundBase* Sound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		ESoundType SoundType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		ESoundLabels SoundLabel;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float VolumeMultiplier = (1.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float PitchMultiplier = (1.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float StartTime = (1.0f);
};

/**
 * Persistent through play session.
 */
UCLASS()
class KEEPTHEPUPPYDRY_API UPGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPGameInstance();

	virtual void Init() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		float MinimumLoadingScreenDisplayTime;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TSubclassOf<class UUserWidget> WidgetLoadingScreenClass;

	UFUNCTION()
		virtual void BeginLoadingScreen(const FString& MapName);
	UFUNCTION()
		virtual void EndLoadingScreen(UWorld* InLoadedWorld);

	bool ShouldSkipToGameplay() const { return bSkipToGameplay; }

	void SetSkipToGameplay(bool bSkip) { bSkipToGameplay = bSkip; }

private:
	class UUserWidget* LoadingScreenWidget;
	
	bool bSkipToGameplay;

public:
	int32 IncrementNumSessionLosses() { return ++NumSessionLosses; }

	int32 GetNumSessionLosses() const { return NumSessionLosses; }

	bool bGameServicesActivated;

private:
	int32 NumSessionLosses;

// AUDIO
public:
	UFUNCTION(BlueprintCallable)
		void SetVolume(float MusicVolume, float SFXVolume);

	UFUNCTION(BlueprintCallable)
		void PlaySound2D(class USoundBase* Sound, ESoundType SoundType, float VolumeMultiplier = 1.0f, float PitchMultiplier = 1.0f, float StartTime = 1.0f);

	UFUNCTION(BlueprintCallable)
		void PlayPersisentSound(ESoundLabels SoundLabel, float FadeInDuration);

	UFUNCTION(BlueprintCallable)
		void StopPersistentSound(ESoundLabels SoundLabel, float FadeOutDuration);

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		float GlobalMusicVolume;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		float GlobalSFXVolume;

	// Stored in enum order
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FPersistentSound> PersistentSounds;

private:
	UPROPERTY()
		TMap<ESoundLabels, class UAudioComponent*> PersistentSoundMap;
};
