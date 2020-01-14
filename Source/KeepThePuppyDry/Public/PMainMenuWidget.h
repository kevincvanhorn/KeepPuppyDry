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

	/** Unpause game & remove this widget.*/
	UFUNCTION(BlueprintCallable)
		void StartGame();

	void Initialize(class APPlayerController* ControllerIn);

protected:
	UFUNCTION()
		void OnGameOver();

	TArray<class UCanvasPanel*> Menus;

	/** Redundant: Hide all screens and show the @param screen. */
	void SetScreenVisible(class UCanvasPanel* ScreenIn);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* MainMenu;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* TitleScreen;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* StartGameButton;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* GameOverScreen;

	class APPlayerController* PPlayerController;
};
