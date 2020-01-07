// Copyright 2020, Kevin VanHorn. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PSwipeDelegates.generated.h"

/**
 * Global delegates. TODO: Update class name.
 */
UCLASS()
class KEEPTHEPUPPYDRY_API UPSwipeDelegates : public UObject
{
	GENERATED_BODY()
public:
	DECLARE_MULTICAST_DELEGATE(FTouchDelegate);

	static FTouchDelegate TouchBeganDelegate;
	static FTouchDelegate TouchEndedDelegate;

	static FTouchDelegate SwipeLeftDelegate;
	static FTouchDelegate SwipeRightDelegate;
	static FTouchDelegate SwipeUpDelegate;
	static FTouchDelegate SwipeDownDelegate;

	static FTouchDelegate SingleTapDelegate;
	static FTouchDelegate DoubleTapDelegate;

public:
	DECLARE_MULTICAST_DELEGATE(FLevelEventDelegate);

	static FLevelEventDelegate PostBeginPlayDelegate;

	static FLevelEventDelegate PuppySitDelegate;
	static FLevelEventDelegate PuppyStandDelegate;

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDifficultyChangedDelegate, int32, Difficulty);

	static FDifficultyChangedDelegate DifficultySwitchDelegate;
};
