// Copyright 2019, Kevin VanHorn. All rights reserved.

#include "PLevelScriptActor.h"
#include "PRainEmitter.h"
#include "PProcedualMeshActor.h"

APLevelScriptActor::APLevelScriptActor() {
	bMovingCylinder = false;
	CylinderLerpSpeed = 1.0f;
}

void APLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();
}

void APLevelScriptActor::SetUmbrellaCylinder(APProcedualMeshActor* CylinderIn)
{
	UmbrellaCylinder = CylinderIn;
	if (UmbrellaCylinder) {
		FVector CapLoc = UmbrellaCylinder->GetCapLocation();
		CurRainDirection = FVector2D(CapLoc.X, CapLoc.Y);
	}
}

void APLevelScriptActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bMovingCylinder) {
		CurRainDirection = FMath::Vector2DInterpTo(CurRainDirection, TargetRainDirection, DeltaTime, CylinderLerpSpeed);
		UmbrellaCylinder->SetCapLocationXY(CurRainDirection);
		if (FVector2D::Distance(TargetRainDirection, CurRainDirection) < 0.01f) {
			bMovingCylinder = false;
		}
	}
}

void APLevelScriptActor::ChangeRainDirection(FVector DirectionIn)
{
	for (APRainEmitter* Emitter : RainEmmitters) {
		Emitter->SetAcceleration(DirectionIn);
	}
	TargetRainDirection = FVector2D(DirectionIn.X, DirectionIn.Y);
	bMovingCylinder = true;
}
