// Copyright 2020, Kevin VanHorn. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGameInstance.h"
#include "PCustomizationManager.h"
#include "PMainMenuWidget.generated.h"

/**
 * Widget that covers the persistent level on game start (constructed before postbeginplay).
 */
UCLASS()
class KEEPTHEPUPPYDRY_API UPMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	/** Unpause game & set the appropriate canvas panel visible.*/
	UFUNCTION(BlueprintCallable)
		void StartGame();

	UFUNCTION(BlueprintCallable)
		void EndTutorial();

	void PInitialize(class APPlayerController* ControllerIn, class APPlayerState* PPlayerStateIn, bool bSkipMenuIn);

	UFUNCTION(BlueprintCallable)
		void SelectUmbrellaPattern(EUmbrellaPattern UmbrellaPattern);

	void SetCustomizationManager(class APCustomizationManager* ManagerIn) { CustomizationManager = ManagerIn; }

protected:
	UFUNCTION()
		void OnGameOver();

	UFUNCTION(BlueprintCallable)
		void EndLoadingScreen();

	UFUNCTION(BlueprintCallable)
		bool bCanDisplayInterstitialAd();

	TArray<class UCanvasPanel*> Menus;

	class APCustomizationManager* CustomizationManager;

	/** Redundant: Hide all screens and show the @param screen. */
	void SetScreenVisible(class UCanvasPanel* ScreenIn);

protected:
	UFUNCTION()
		void OnTouchBegin();

	UFUNCTION()
		void OnTouchEnd();

	UFUNCTION()
		void PostBeginPlay();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnSkipMenu"))
		void OnSkipMenu();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnPostBeginPlay"))
		void OnPostBeginPlay();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnTouchBegin"))
		void OnBPTouchBegin();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnTouchEnd"))
		void OnBPTouchEnd();

	UFUNCTION(BlueprintCallable)
		void PRestartLevel(bool bShowTitleScreen);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* MainMenu;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* TitleScreen;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* TutorialScreen;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* CountdownScreen;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* StartGameButton;

	class APPlayerController* PPlayerController;

	class APPlayerState* PPlayerState;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		bool bSkipMenus;

private:
	bool bTutorialInProgress;
	
	class UPGameInstance* PGameInstance;

// GAME OVER
public:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnGameOver"))
		void OnGameOverEventBP();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* GameOverScreen;

// SOUNDS
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* MuteButton;

	UFUNCTION(BlueprintCallable)
		void PlaySound2D(class USoundBase* Sound, ESoundType SoundType, float VolumeMultiplier = 1.0f, float PitchMultiplier = 1.0f, float StartTime = 0.0f);

	UFUNCTION(BlueprintCallable)
		void PlayPersisentSound(ESoundLabels SoundLabel, float FadeInDuration);

	UFUNCTION(BlueprintCallable)
		void StopPersistentSound(ESoundLabels SoundLabel, float FadeOutDuration);

	UFUNCTION(BlueprintCallable)
		void SetVolume(float MusicVolume, float SFXVolume);

	/** Return true if the volume is turned on. */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "OnMuteButtonPressed"))
		bool OnMuteButtonPressedBP();

	/** Material for Volume On. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UObject* SoundOnImageObj;

	/** Material for Volume Off. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UObject* SoundOffImageObj;

	private:
		bool bVolumeOn;
};
