// Fill out your copyright notice in the Description page of Project Settings.


#include "PUmbrella.h"

APUmbrella::APUmbrella() {
	bMoving = false;
	PrimaryActorTick.bCanEverTick = true; //We won't be ticked by default  
}

void APUmbrella::MoveToPosition(FVector Target)
{
	bMoving = true;
	TargetPosition = Target;
}

void APUmbrella::Tick(float DeltaTime)
{
	if (bMoving) {
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), TargetPosition, DeltaTime, MoveSpeed));
		if (FVector::Dist(TargetPosition, GetActorLocation()) < 0.05f) bMoving = false;
	}
}