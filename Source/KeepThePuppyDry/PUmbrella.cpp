// Fill out your copyright notice in the Description page of Project Settings.

#include "PUmbrella.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Engine/World.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "PPlayerController.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "KeepThePuppyDry.h"
#include "PProcedualMeshActor.h"

APUmbrella::APUmbrella() {
	bMoving = false;
	PrimaryActorTick.bCanEverTick = true; //We won't be ticked by default 
}

void APUmbrella::BeginPlay()
{
	Super::BeginPlay();

	if (CylinderMeshClass) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		CylinderMesh = (APProcedualMeshActor*)GetWorld()->SpawnActor<APProcedualMeshActor>(CylinderMeshClass, CylinderOffset, FRotator(180, 0, 0), SpawnParams);
		if (CylinderMesh) {
			CylinderMesh->SetUParent(this); 
			CylinderMesh->SetUOffset(CylinderOffset);
		}
	}
}

FVector APUmbrella::ScreenToWorldLoc(FVector2D ScreenLoc)
{
	FVector Intersect;
	if (PController) {
		FVector WorldLoc = FVector::ZeroVector;
		FVector WorldDir;
		UGameplayStatics::DeprojectScreenToWorld(PController, ScreenLoc, WorldLoc, WorldDir);

		// Get Plane intersection of line from camera.
		FVector PNormal = -1 * GetActorForwardVector();

		float Dist = (FVector::DotProduct(PNormal, GetActorLocation() - WorldLoc) / FVector::DotProduct(PNormal, WorldDir));
		Intersect = WorldLoc + WorldDir * Dist;
		DrawDebugSphere(GetWorld(), Intersect, 2.0f, 4, FColor::Red, false, 0.1f);
		//TargetZ = FMath::Clamp(Intersect.Z, ClampZPos, ClampZNeg);
	}
	return Intersect;
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
	TargetPosition = UReleasePosition;
	//TargetZ = UReleasePosition.Z;
}

void APUmbrella::LerpToPosition(FVector Target)
{
	bMoving = true;
	TargetPosition = Target;
}

void APUmbrella::MoveFromScreenLoc(FVector2D ScreenLoc)
{
	FVector Intersect = ScreenToWorldLoc(ScreenLoc);
	TargetPosition = FVector(TargetPosition.X, Intersect.Y, TargetPosition.Z);
	bMoving = true;
	SetActorLocation(TargetPosition);
	/*if (!bMoving) {
		SetActorLocation(TargetPosition);
	}*/
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
			if (TargetPosition == UTouchPosition && B_LEVEL_SIMPLE) {
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

void APUmbrella::OnTouchBegin(FVector2D ScreenLoc)
{
	MoveSpeed = MaxMoveSpeed;
	LerpToPosition(FVector(TargetPosition.X, ScreenToWorldLoc(ScreenLoc).Y, TargetPosition.Z));
}

void APUmbrella::OnTouchEnd()
{
	MoveSpeed = ReleaseSpeed;
	LerpToPosition(FVector(UReleasePosition.X, UReleasePosition.Y, UReleasePosition.Z));
}