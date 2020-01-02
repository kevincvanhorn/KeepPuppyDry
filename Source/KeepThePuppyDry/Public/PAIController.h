// Copyright 2020, Kevin VanHorn. All rights reserved.

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
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
		void DoNextAction();

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

protected:
	FTimerHandle AITimerHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float ActionDelay;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float WaitProbability;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float SitProbability;

private:
	class UNavigationSystemV1* Navigation;

	class APPuppyCharacter* Puppy;

	float TimeSinceLastAction;
};
