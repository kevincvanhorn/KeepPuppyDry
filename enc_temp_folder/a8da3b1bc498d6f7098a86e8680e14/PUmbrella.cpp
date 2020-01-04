// Copyright 2020, Kevin VanHorn. All rights reserved.

#include "PUmbrella.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Engine/World.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "PPlayerController.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "KeepThePuppyDry.h"
#include "PProcedualMeshActor.h"
#include "PLevelScriptActor.h"
#include "Components/SphereComponent.h"

APUmbrella::APUmbrella() {
	bMoving = false;
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("BotSphereComponent"));
	if (SphereComponent) {
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APUmbrella::OnOverlapBegin);
		SphereComponent->OnComponentEndOverlap.AddDynamic(this, &APUmbrella::OnOverlapEnd);
	}
	FloorLocationZ = 140.0f;
}

void APUmbrella::BeginPlay()
{
	Super::BeginPlay();

	Level = (APLevelScriptActor*)GetWorld()->GetLevelScriptActor();

	if (CylinderMeshClass) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		CylinderMesh = (APProcedualMeshActor*)GetWorld()->SpawnActor<APProcedualMeshActor>(CylinderMeshClass, CylinderSpawnLocation, FRotator(180, 0, 0), SpawnParams);
		if (CylinderMesh) {
			CylinderMesh->SetUParent(this); 
			CylinderMesh->SetUOffset(CylinderSpawnLocation);
			if (Level) {
				Level->SetUmbrellaCylinder(CylinderMesh);
			}
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

FVector APUmbrella::GetSphereLocation() const
{
	if (SphereComponent) {
		return SphereComponent->GetComponentLocation();
	}
	return FVector();
}

void APUmbrella::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OverlapBegin %s"), *OtherActor->GetName());
}

void APUmbrella::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("OverlapEnd %s"), *OtherActor->GetName());
}

// PostBeginPlay - called from Controller
void APUmbrella::Initialize(FVector UTouchPositionIn, FVector UReleasePositionIn, APPlayerController* PControllerIn)
{
	UTouchPosition = UTouchPositionIn;
	UReleasePosition = UReleasePositionIn;
	PController = PControllerIn;

	if (MPC) {
		FVector Cur = GetActorLocation();
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC, FName("ULocation"), FLinearColor(Cur.X, Cur.Y, FloorLocationZ, 0));
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
	FVector Cur = GetActorLocation();
	FVector GroundLoc = FVector(Cur.X, Cur.Y, FloorLocationZ); // Location where the bottom of the cylinder is projected
	if (CylinderMesh) {
		CylinderMesh->ProjectCapLocation(GroundLoc, FloorLocationZ);
	}

	// Set location to block particles bottom of cylinder.
	if (SphereComponent) {
		SphereComponent->SetWorldLocation(GroundLoc);
	}
	// MPC for grass material
	if (MPC) {
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC, FName("ULocation"), FLinearColor(GroundLoc.X, GroundLoc.Y, GroundLoc.Z, 0));
	}

	// Respond to Touch Input
	if (bMoving) {
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), TargetPosition, DeltaTime, MoveSpeed));
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