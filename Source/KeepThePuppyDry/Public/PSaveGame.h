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
	/** uint64s are masks to store up to 64 one hot encodings. */
	static bool SynchronousSave(int32 CurrencyIn, bool bShowTutorialIn, const TArray<uint8>& UPatternsIn, int32 LS_UmbrellaPatternIn,
		const TArray<uint8>& DogChoicesIn, int32 LS_DogIn);

public:
	UPROPERTY(VisibleAnywhere, Category = Basic)
		int32 Currency;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		bool bShowTutorial;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		TArray<uint8> UPatterns;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		int32 LS_UmbrellaPattern;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		TArray<uint8> DogChoices;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		int32 LS_Dog;
public:
	static FString SaveSlotName;

	static uint32 UserIndex;
};
