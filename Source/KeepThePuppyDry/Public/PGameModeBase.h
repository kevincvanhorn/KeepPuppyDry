// Copyright 2019, Kevin VanHorn. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class KEEPTHEPUPPYDRY_API APGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	APGameModeBase();

	virtual void StartPlay();
	
protected:
	class UUserWidget* PLoadingScreen;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UUserWidget> PLoadingScreenClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bSpawnLoadingScreen;

	UFUNCTION()
		void EndLoadingScreen();
};
