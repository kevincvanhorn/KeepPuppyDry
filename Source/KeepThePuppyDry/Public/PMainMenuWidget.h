// Copyright 2020, Kevin VanHorn. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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

	void PInitialize(class APPlayerController* ControllerIn);

protected:
	UFUNCTION()
		void OnGameOver();

	UFUNCTION(BlueprintCallable)
		void EndLoadingScreen();


	TArray<class UCanvasPanel*> Menus;

	/** Redundant: Hide all screens and show the @param screen. */
	void SetScreenVisible(class UCanvasPanel* ScreenIn);

protected:
	UFUNCTION()
		void OnTouchBegin();

	UFUNCTION()
		void OnTouchEnd();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnTouchBegin"))
		void OnBPTouchBegin();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnTouchEnd"))
		void OnBPTouchEnd();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* MainMenu;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* TitleScreen;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* TutorialScreen;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* StartGameButton;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* GameOverScreen;

	class APPlayerController* PPlayerController;

private:
	bool bTutorialInProgress;
};
