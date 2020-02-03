// Copyright 2020, Kevin VanHorn. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ScaleBox.h"
#include "PScaleBox.generated.h"

/**
 * 
 */
UCLASS()
class KEEPTHEPUPPYDRY_API UPScaleBox : public UScaleBox
{
	GENERATED_BODY()

public:
	UPScaleBox();

	/** Lerps this widget to the target position, starting from the beginning if already moving. */
	void MoveTo(FVector2D StartLoc, FVector2D TargetLocIn, float InterpSpeed);

protected:
	UFUNCTION()
		void UpdateLocation();

private:
	FTimerHandle MoveHandle;

	float MoveSpeed;

	FVector2D TargetLoc;
};
