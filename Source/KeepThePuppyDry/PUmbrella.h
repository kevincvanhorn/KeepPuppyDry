// Copyright 2020, Kevin VanHorn. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "PUmbrella.generated.h"

/**
 * 
 */
UCLASS()
class KEEPTHEPUPPYDRY_API APUmbrella : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	APUmbrella();

	void Initialize(class APPlayer* PlayerIn, FVector UTouchPositionIn, FVector UReleasePositionIn, class APPlayerController* PControllerIn);

	void LerpToPosition(FVector Target);
	
	// Use screen location to move umbrella.
	void MoveFromScreenLoc(FVector2D ScreenLoc);

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Umbrella")
		float MaxMoveSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Umbrella")
		float ReleaseSpeed;

	void SetMPC(class UMaterialParameterCollection* MPC_In);

	void OnTouchBegin(FVector2D ScreenLoc);

	void OnTouchEnd();

	void SetClampZValues(float Pos, float Neg) { ClampZPos = Pos; ClampZNeg = Neg; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnTouchLoc"))
		void OnTouchLoc();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnReleaseLoc"))
		void OnReleaseLoc();

	FVector ScreenToWorldLoc(FVector2D ScreenLoc);

protected:
	class APPlayerController* PController;

	class APLevelScriptActor* Level;

	class APPlayer* PPlayer;

private:
	class UMaterialParameterCollection* MPC;

	FVector UTouchPosition;
	FVector UReleasePosition;

	bool bMoving;
	FVector TargetPosition;

private:
	float ClampZPos;
	float ClampZNeg;
	float TargetZ;
	float MoveSpeed;

// God Ray
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TSubclassOf<class APProcedualMeshActor> CylinderMeshClass;
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector CylinderSpawnLocation;

protected:
	class APProcedualMeshActor* CylinderMesh;

// Bottom sphere for particle collision:
public:
	/** Get the location of the pawn overlapping sphere in world space.*/
	UFUNCTION(BlueprintCallable)
		FVector GetSphereLocation() const;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float FloorLocationZ;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class USphereComponent* SphereComponent;

	/** called when something enters the sphere component */
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** called when something leaves the sphere component */
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Change Size:
	public:
		UFUNCTION(BlueprintCallable)
			void ChangeUmbrellaSizeScalar(float NewSize);

		/** Return true if a difficulty increase was achieved (i.e. diff cap not reached). */
		UFUNCTION(BlueprintCallable)
			bool IncreaseDifficulty();

	protected:
		/** Difficulty Effects per level. */
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
			TArray<float> DifficultyLevels;

		/** Seconds between updates to size. */
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
			float USizeTimerRate;

		/** Speed of the vInterp for the umbrella size. */
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
			float USizeInterpSpeed;

	private:
		UFUNCTION()
			void UpdateUmbrellaSize();
		
		FTimerHandle USizeHandle;
		FVector UTargetSize;

		float USizeDefault_MPC; // Radius of material sphere mask

		float DefaultSphereRadius;



// Gameplay Stats
private:

	int32 Difficulty;

// Overlap Percentage:
public:
	float GetOverlapPercentage() const { return OverlapPercentage; }

private:

	bool bOverlappingPuppy;

	float OverlapPercentage;
	
	float PuppyRadius; // Radius of capsule.

	class APPuppyCharacter* Puppy;
};
