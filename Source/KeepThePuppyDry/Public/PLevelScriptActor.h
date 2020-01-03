// Copyright 2019, Kevin VanHorn. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "PLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class KEEPTHEPUPPYDRY_API APLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	APLevelScriptActor();

	void SetUmbrellaCylinder(class APProcedualMeshActor* CylinderIn);

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void ChangeRainDirection(FVector DirectionIn);
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<class APRainEmitter*> RainEmmitters;

	class APProcedualMeshActor* UmbrellaCylinder;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float RainLerpSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float CylinderLerpSpeed;

	/** Modifier for magnitude of Cylinder Direction.*/ // TODO: Rename.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float RainSpeed;

	/** Modifier for magnitude of Rain velocity.*/ 
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float RainIntensity;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int EmitterUpdatePerNumFrames;

	/** [0-1] Modifier for aligning cylinder at lower speeds.*/ // TODO
	/*UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float CylinderOffset;*/

private:
	FVector2D CurRainDirection2D;

	FVector2D TargetRainDirection2D;
	FVector CurRainDirection;
	FVector TargetRainDirection;

	int FramesSinceEmitterUpdate;

	bool bMovingCylinder;
};
