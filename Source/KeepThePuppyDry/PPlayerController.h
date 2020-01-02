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
		float MinSwipeDistance;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		float MaxDoubleTapDelay;

	FVector2D GetLastTouchLocation() const { return LastTouchLoc; }

protected:
	UPUserWidget* PUserWidget;

protected:
	virtual bool InputTouch(uint32 Handle, ETouchType::Type Type, const FVector2D& TouchLocation, float Force, FDateTime DeviceTimestamp, uint32 TouchpadIndex) override;

	virtual void SetupInputComponent() override;

	/* Called before BeginPlay(), only during game execution. */
	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	bool bIgnoreInput;

	class APPlayer* PPlayer;

private:
	FVector2D SwipeStartLoc;

	FVector2D LastTouchLoc;

	bool bIsSwiping;

	Swipe::Direction SwipeDirection;

	int32 TapCount;

	UFUNCTION()
		void ResetTapHandler();
};
