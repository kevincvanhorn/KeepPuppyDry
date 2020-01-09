// Copyright 2019, Kevin VanHorn. All rights reserved.

#include "PLevelScriptActor.h"
#include "PRainEmitter.h"
#include "PProcedualMeshActor.h"
#include "PSwipeDelegates.h"
#include "PAIController.h"
#include "PUmbrella.h"

APLevelScriptActor::APLevelScriptActor() {
	bMovingCylinder = false;
	RainLerpSpeed = 1.0f;
	CylinderLerpSpeed = 1.0f;
	RainSpeed = 1;
	EmitterUpdatePerNumFrames = 3;
}

void APLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	UPSwipeDelegates::DifficultySwitchDelegate.AddUniqueDynamic(this, &APLevelScriptActor::OnDifficultyChangedInternal);
}

void APLevelScriptActor::OnDifficultyChangedInternal(int32 NewDifficulty)
{
	this->OnDifficultyChanged(NewDifficulty);
}

void APLevelScriptActor::IncreaseDifficulty(EDifficultyType DiffType)
{
	if (DiffType == EDifficultyType::EDog && AIController) {
		AIController->IncreaseDifficulty();
	}
	else if (DiffType == EDifficultyType::ERain) {
		
	}
	else if (DiffType == EDifficultyType::EUmbrella && Umbrella) {
		Umbrella->IncreaseDifficulty();
	}
}

void APLevelScriptActor::SetUmbrellaCylinder(APProcedualMeshActor* CylinderIn)
{
	UmbrellaCylinder = CylinderIn;
	if (UmbrellaCylinder) {
		FVector CapLoc = UmbrellaCylinder->GetCapLocation();
		CurRainDirection2D = FVector2D(CapLoc.X, CapLoc.Y);
	}
}

void APLevelScriptActor::SetUmbrella(APUmbrella* UmbrellaIn)
{
	Umbrella = UmbrellaIn;
}

void APLevelScriptActor::SetAIController(APAIController* AIControllerIn)
{
	AIController = AIControllerIn;
}

void APLevelScriptActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bMovingCylinder && UmbrellaCylinder) {
		CurRainDirection2D = FMath::Vector2DInterpTo(CurRainDirection2D, TargetRainDirection2D, DeltaTime, CylinderLerpSpeed);
		//CurRainDirection = FMath::VInterpTo(CurRainDirection, TargetRainDirection, DeltaTime, RainLerpSpeed);

		UmbrellaCylinder->SetCapLocationXY(CurRainDirection2D*RainSpeed);
		if (FVector2D::Distance(TargetRainDirection2D, CurRainDirection2D) < 0.01f) {
			bMovingCylinder = false;
		}

		/*
		FramesSinceEmitterUpdate++;
		if (FramesSinceEmitterUpdate > EmitterUpdatePerNumFrames) {
			FramesSinceEmitterUpdate = 0;
			for (APRainEmitter* Emitter : RainEmmitters) {
				if (Emitter) {
					Emitter->SetAcceleration(CurRainDirection, RainIntensity);
				}
			}
		}
		*/
	}
}

void APLevelScriptActor::ChangeRainDirection(FVector DirectionIn)
{
	for (APRainEmitter* Emitter : RainEmmitters) {
		if (Emitter) {
			Emitter->SetAcceleration(DirectionIn, RainIntensity);
		}
	}
	//TargetRainDirection = DirectionIn;
	TargetRainDirection2D = FVector2D(DirectionIn.X, DirectionIn.Y);
	bMovingCylinder = true;
}