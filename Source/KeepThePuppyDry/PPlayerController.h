// Copyright 2020, Kevin VanHorn. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PPlayerController.generated.h"

UENUM(BlueprintType)
namespace Swipe {
	enum Direction {
		None,
		Left,
		Right,
		Up,
		Down
	};
}

/**
 * 
 */
UCLASS()
class KEEPTHEPUPPYDRY_API APPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	APPlayerController();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TSubclassOf<class UPUserWidget> PUserWidgetClass;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TSubclassOf<class UPMainMenuWidget> PMainMenuWidgetClass;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		float MinSwipeDistance;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		float MaxDoubleTapDelay;

	FVector2D GetLastTouchLocation() const { return LastTouchLoc; }

	/** Called from MainMenuWidget to Unpause & postinitialize game. 
		@return tutorial enabled? */
	bool StartGame(bool bSkipStartOverride = false);

	void EndTutorial();

	UFUNCTION()
		void OnGameOver();

protected:
	class APLevelScriptActor* PLevel;

	UPUserWidget* PUserWidget;

	class UPMainMenuWidget* PMainMenuWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player, Managers", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class APDifficultyManager> DifficultyManagerClass;

protected:
	virtual bool InputTouch(uint32 Handle, ETouchType::Type Type, const FVector2D& TouchLocation, float Force, FDateTime DeviceTimestamp, uint32 TouchpadIndex) override;

	virtual void SetupInputComponent() override;

	/* Called before BeginPlay(), only during game execution. */
	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	bool bIgnoreInput;

	class APPlayer* PPlayer;

	class APPlayerState* PPlayerState;

	class UPGameInstance* PGameInstance;

	class APDifficultyManager* DifficultyManager;

	class APCustomizationManager* CustomizationManager;

private:
	FVector2D SwipeStartLoc;

	FVector2D LastTouchLoc;

	bool bIsSwiping;

	Swipe::Direction SwipeDirection;

	int32 TapCount;

	bool bShowTutorial;

	UFUNCTION()
		void ResetTapHandler();

	//UWorld* World;
};
