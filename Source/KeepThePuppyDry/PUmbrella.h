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

	void Initialize(FVector UTouchPositionIn, FVector UReleasePositionIn);

	void MoveToPosition(FVector Target);

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Umbrella")
		float MoveSpeed;

	void SetMPC(class UMaterialParameterCollection* MPC_In);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnTouchLoc"))
		void OnTouchLoc();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnReleaseLoc"))
		void OnReleaseLoc();

private:
	class UMaterialParameterCollection* MPC;

	FVector UTouchPosition;
	FVector UReleasePosition;

	bool bMoving;
	FVector TargetPosition;
};
