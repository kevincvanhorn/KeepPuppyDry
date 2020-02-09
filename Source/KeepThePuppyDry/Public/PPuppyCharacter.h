// Copyright 2020, Kevin VanHorn. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PPuppyCharacter.generated.h"

UCLASS()
class KEEPTHEPUPPYDRY_API APPuppyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APPuppyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UCharacterMovementComponent* MovementComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnGetWet"))
		void OnGetWet();

// Navigation: ------------------------
public:
	/** Get a y-world aligned random navigatable location a min distance away from the pawn location.*/
	UFUNCTION(BlueprintCallable)
		FVector GetNextRandomLocation();

protected:
	/** Center of the scene origin in world-space.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PNavigation")
		FVector NavOrigin;

	/** Radius from the scene origin - max distance this pawn can move.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PNavigation")
		float NavRadius;

	/** Min distance from the last location the pawn can move for navigation. */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "PNavigation")
		float NavMinMoveDist;

private:
	/** Takes a value in/out of a range and wraps it back into the range. */
	float WrapToNavRange(const float Loc);

	float NavRangeMin;
	float NavRangeMax;


// Difficulty
public:
	void OnIncreaseDifficulty(float NavMinMoveDistIn, float SpeedIn, float AccelIn);
};
