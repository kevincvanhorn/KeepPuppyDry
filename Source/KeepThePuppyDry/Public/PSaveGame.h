// Copyright 2020, Kevin VanHorn. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PCustomizationManager.h"
#include "PSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class KEEPTHEPUPPYDRY_API UPSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	static bool SynchronousSave(int32 CurrencyIn, bool bShowTutorialIn);

public:
	UPROPERTY(VisibleAnywhere, Category = Basic)
		int32 Currency;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		bool bShowTutorial;

public:
	static FString SaveSlotName;

	static uint32 UserIndex;
};
