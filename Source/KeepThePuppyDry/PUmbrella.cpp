// Fill out your copyright notice in the Description page of Project Settings.

#include "PUmbrella.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Engine/World.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "PPlayerController.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"


APUmbrella::APUmbrella() {
	bMoving = false;
	PrimaryActorTick.bCanEverTick = true; //We won't be ticked by default  
}

void APUmbrella::BeginPlay()
{
	Super::BeginPlay();
}

// PostBeginPlay - called from Controller
void APUmbrella::Initialize(FVector UTouchPositionIn, FVector UReleasePositionIn, APPlayerController* PControllerIn)
{
	UTouchPosition = UTouchPositionIn;
	UReleasePosition = UReleasePositionIn;
	PController = PControllerIn;

	if (MPC) {
		FVector Cur = GetActorLocation();
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC, FName("ULocation"), FLinearColor(Cur.X, Cur.Y, 140, 0));
	}

	SetActorLocation(UReleasePosition);
}

void APUmbrella::MoveToPosition(FVector Target)
{
	bMoving = true;
	TargetPosition = Target;
}

void APUmbrella::MoveFromScreenLoc(FVector2D ScreenLoc)
{
	if (PController) {
		FVector WorldLoc = FVector::ZeroVector;
		FVector WorldDir;
		UGameplayStatics::DeprojectScreenToWorld(PController, ScreenLoc, WorldLoc, WorldDir);
		//DrawDebugSphere(GetWorld(), WorldLoc, 0.1f, 12, FColor::Red, true, 10.0f);
	}
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


