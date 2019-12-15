// Fill out your copyright notice in the Description page of Project Settings.

#include "PUmbrella.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Engine/World.h"

APUmbrella::APUmbrella() {
	bMoving = false;
	PrimaryActorTick.bCanEverTick = true; //We won't be ticked by default  
}

void APUmbrella::Initialize(FVector UTouchPositionIn, FVector UReleasePositionIn)
{
	UTouchPosition = UTouchPositionIn;
	UReleasePosition = UReleasePositionIn;

	if (MPC) {
		FVector Cur = GetActorLocation();
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC, FName("ULocation"), FLinearColor(Cur.X, Cur.Y, 140, 0));
	}
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
		if (MPC) {
			FVector Cur = GetActorLocation();
			
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC, FName("ULocation"), FLinearColor(Cur.X, Cur.Y, 140,0));
		}
		if (FVector::Dist(TargetPosition, GetActorLocation()) < 0.05f) {
			bMoving = false;
			if (TargetPosition == UTouchPosition) {
				this->OnTouchLoc();
			}
			else if (TargetPosition == UReleasePosition) {
				this->OnReleaseLoc();
			}
		}
	}
}

void APUmbrella::SetMPC(UMaterialParameterCollection* MPC_In)
{
	MPC = MPC_In;
}
