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
	/** Unpause game & remove this widget.*/
	UFUNCTION(BlueprintCallable)
		void StartGame();

	void Initialize(class APPlayerController* ControllerIn);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* StartGameWidget; // TODO rename here and in BP to Button

	class APPlayerController* PPlayerController;
};
