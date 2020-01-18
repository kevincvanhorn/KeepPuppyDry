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
#include "PPuppyCharacter.h"
#include "PPlayer.h"
#include "TimerManager.h"
#include "PSwipeDelegates.h"
#include "Components/CapsuleComponent.h"

APUmbrella::APUmbrella() {
	bMoving = false;
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("BotSphereComponent"));
	if (SphereComponent) {
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APUmbrella::OnOverlapBegin);
		SphereComponent->OnComponentEndOverlap.AddDynamic(this, &APUmbrella::OnOverlapEnd);
	}
	FloorLocationZ = 140.0f;
	USizeTimerRate = 0.1f;
	Difficulty = 0;
	bOverlappingPuppy = false;
	DefaultSphereRadius = 280.0f;
	PuppyRadius = 1.0f;
}

void APUmbrella::BeginPlay()
{
	Super::BeginPlay();

	UPSwipeDelegates::GameOverDelegate.AddUObject(this, &APUmbrella::OnGameOver);
	UPSwipeDelegates::GameStartDelegate.AddUObject(this, &APUmbrella::OnGameStart);
	UPSwipeDelegates::GameTutorialDelegate.AddUObject(this, &APUmbrella::OnGameTutorial);

	DefaultSphereRadius = SphereComponent->GetUnscaledSphereRadius();
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
				Level->SetUmbrella(this);
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
		//DrawDebugSphere(GetWorld(), Intersect, 2.0f, 4, FColor::Red, false, 0.1f);
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
	APPuppyCharacter* PuppyOverlap =  dynamic_cast<APPuppyCharacter*>(OtherActor);
	if (PuppyOverlap && PPlayer) {
		if (!Puppy) {
			Puppy = PuppyOverlap;
			UCapsuleComponent* Capsule =  Puppy->GetCapsuleComponent();
			if (Capsule) {
				PuppyRadius = Capsule->GetScaledCapsuleRadius();
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("OverlapBegin %s"), *OtherActor->GetName());
		PPlayer->OnUmbrellaOverlapBegin();
		bOverlappingPuppy = true;
	}
}

void APUmbrella::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APPuppyCharacter* PuppyOverlap = dynamic_cast<APPuppyCharacter*>(OtherActor);
	if (PuppyOverlap && PPlayer) {
		UE_LOG(LogTemp, Warning, TEXT("OverlapEnd %s"), *OtherActor->GetName());
		PPlayer->OnUmbrellaOverlapEnd();
		bOverlappingPuppy = false;
		OverlapPercentage = 0.0f;
	}
}

// PostBeginPlay - called from Controller
void APUmbrella::Initialize(APPlayer* PlayerIn,  FVector UTouchPositionIn, FVector UReleasePositionIn, APPlayerController* PControllerIn)
{
	UTouchPosition = UTouchPositionIn;
	UReleasePosition = UReleasePositionIn;
	PController = PControllerIn;
	PPlayer = PlayerIn;

	if (MPC) {
		FVector Cur = GetActorLocation();
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC, FName("ULocation"), FLinearColor(Cur.X, Cur.Y, FloorLocationZ, 0));
		USizeDefault_MPC = UKismetMaterialLibrary::GetScalarParameterValue(GetWorld(), MPC, FName("USize"));
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
		
		// Check Overlap Percentage:
		if (bOverlappingPuppy && Puppy) {
			// Ru-Rp / d  :assuming Rp is less than Ru (p = puppy, u = umbrella, d = dist from centers)
			OverlapPercentage = FMath::Clamp(FMath::Abs(SphereComponent->GetScaledSphereRadius() - PuppyRadius)/ FMath::Abs(GroundLoc.Y - Puppy->GetActorLocation().Y), 0.0f, 1.0f);
			UE_LOG(LogTemp, Warning, TEXT("OverlapEnd %f"), OverlapPercentage);
		}
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

void APUmbrella::OnGameTutorial()
{
	SetActorLocation(FVector(0.0f,0.0f, UReleasePosition.Z));
	TargetPosition = FVector(0.0f, 0.0f, UReleasePosition.Z);
	bMoving = false;

	if (SphereComponent){
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void APUmbrella::OnGameStart()
{
	if (SphereComponent) {
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void APUmbrella::OnGameOver()
{
}

void APUmbrella::ChangeUmbrellaSizeScalar(float NewSize) {
	UTargetSize = FVector(NewSize, NewSize, NewSize);
	GetWorldTimerManager().SetTimer(USizeHandle, this, &APUmbrella::UpdateUmbrellaSize, USizeTimerRate, true, 0.0f);
}

bool APUmbrella::IncreaseDifficulty()
{
	if (Difficulty < DifficultyLevels.Num()) {
		ChangeUmbrellaSizeScalar(DifficultyLevels[Difficulty]);
		Difficulty++;
		return true;
	}
	return false;
}

void APUmbrella::UpdateUmbrellaSize() {
	FVector CurSize = FMath::VInterpTo(GetActorScale3D(), UTargetSize, USizeTimerRate, USizeInterpSpeed);
	
	if (FVector::Dist(CurSize, GetActorScale3D()) > 0.001f) {
		SetActorScale3D(CurSize);
		if (MPC && CylinderMesh) {
			CylinderMesh->SetOffsetScale3D(CurSize);
			float NewRadius = CylinderMesh->GetScaledRadius();
			UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC, FName("USize"), NewRadius);
		}
		if (SphereComponent) {
			SphereComponent->SetSphereRadius(DefaultSphereRadius * GetActorScale().Y);
		}
	}
	else {
		GetWorldTimerManager().ClearTimer(USizeHandle);
	}

}