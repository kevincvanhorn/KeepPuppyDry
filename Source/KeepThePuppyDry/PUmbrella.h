// Fill out your copyright notice in the Description page of Project Settings.

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

	void Initialize(FVector UTouchPositionIn, FVector UReleasePositionIn, class APPlayerController* PControllerIn);

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector CylinderOffset;


protected:
	class APProcedualMeshActor* CylinderMesh;
};
