// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PAIController.generated.h"

/**
 * 
 */
UCLASS()
class KEEPTHEPUPPYDRY_API APAIController : public AAIController
{
	GENERATED_BODY()

public:
	APAIController();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FVector> Waypoints;

	UFUNCTION(BlueprintCallable)
		FVector GetRandomWaypoint();

	UFUNCTION()
		void GoToRandomWaypoint();
};
