// Copyright 2019, Kevin VanHorn. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "PLevelScriptActor.generated.h"

UENUM(BlueprintType)
enum class EDifficultyType : uint8
{
	ERain 	UMETA(DisplayName = "Rain"),
	EDog 		UMETA(DisplayName = "Dog"),
	EUmbrella	UMETA(DisplayName = "Umbrella")
};

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

	void SetUmbrella(class APUmbrella* UmbrellaIn);

	void SetAIController(class APAIController* AIControllerIn);

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void ChangeRainDirection(FVector DirectionIn);

	UFUNCTION()
		void OnDifficultyChangedInternal(int32 NewDifficulty);
	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnDifficultyChanged"))
		void OnDifficultyChanged(int32 DifficultyOut);

	UFUNCTION(BlueprintCallable)
		void IncreaseDifficulty(EDifficultyType DiffType);

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<class APRainEmitter*> RainEmmitters;

	class APProcedualMeshActor* UmbrellaCylinder;

	class APUmbrella* Umbrella;

	class APAIController* AIController;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float RainLerpSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float CylinderLerpSpeed;

	/** Modifier for magnitude of Cylinder Direction.*/ // TODO: Rename.
	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//	float RainCylinderModifier;

	/** Modifier for rain velocity direction.*/
	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//	float RainVelIntensity;

	/** Modifier for magnitude of Rain speed & acceleration.*/ 
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float RainIntensity;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int EmitterUpdatePerNumFrames;

	void OnIncreaseDifficulty_Rain();

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
