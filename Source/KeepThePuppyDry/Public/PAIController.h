// Copyright 2020, Kevin VanHorn. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PAIController.generated.h"

USTRUCT(BlueprintType)
struct FAIDIfficultyStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float ActionDelay;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float WaitProbability;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float SitProbability;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float NavMinMoveDist;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float PSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float PAccel;
};

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
	UFUNCTION()
		void OnGameTutorial();

	UFUNCTION()
		void OnGameStart();

	UFUNCTION()
		void OnGameOver();

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

	class APLevelScriptActor* Level;

private:
	class UNavigationSystemV1* Navigation;

	class APPuppyCharacter* Puppy;

	float TimeSinceLastAction;

	// Difficulty:
public:
	/** Return true if a difficulty increase was achieved (i.e. diff cap not reached). */
	UFUNCTION(BlueprintCallable)
		bool IncreaseDifficulty();

protected:
	/** Difficulty Effects per level. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TArray<FAIDIfficultyStruct> DifficultyLevels;

private:
	int32 Difficulty;
};
