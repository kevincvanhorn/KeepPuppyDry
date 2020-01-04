// Copyright 2019, Kevin VanHorn. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class KEEPTHEPUPPYDRY_API APPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	APPlayerState();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Score")
		float ScoreMultiplier;

	UFUNCTION(BlueprintCallable)
		int32 ScoreFromTime(float TotalTime);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
