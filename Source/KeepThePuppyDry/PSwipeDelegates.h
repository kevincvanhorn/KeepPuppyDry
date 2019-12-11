// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PSwipeDelegates.generated.h"

/**
 * 
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
};
