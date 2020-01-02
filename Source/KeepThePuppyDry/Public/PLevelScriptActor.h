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

	float CylinderLerpSpeed;

private:
	FVector2D CurRainDirection;

	FVector2D TargetRainDirection;

	bool bMovingCylinder;
};
