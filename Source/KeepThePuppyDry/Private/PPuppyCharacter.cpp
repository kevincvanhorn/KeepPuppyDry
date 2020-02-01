// Copyright 2020, Kevin VanHorn. All rights reserved.

#include "PPuppyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "PSwipeDelegates.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APPuppyCharacter::APPuppyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Navigation
	NavMinMoveDist = 100;
	NavRadius = 400;
	NavRangeMin = NavOrigin.Y - NavRadius;
	NavRangeMax = NavOrigin.Y + NavRadius;
}

// Called when the game starts or when spawned
void APPuppyCharacter::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent = GetCharacterMovement();
}


void APPuppyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APPuppyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FVector APPuppyCharacter::GetNextRandomLocation()
{
	float PosY = UKismetMathLibrary::RandomFloatInRange(0, NavMinMoveDist*.05f); // Assumes global origin alignment.
	FVector CurLoc = GetActorLocation();

	/*if (FMath::Abs(CurLoc.Y - PosY) < NavMinMoveDist) {
		return FVector(CurLoc.X, PosY, CurLoc.Z);
	}
	else {*/
		int Dir = UKismetMathLibrary::RandomBool()? 1 : -1;
		return FVector(CurLoc.X, WrapToNavRange(CurLoc.Y + Dir * (NavMinMoveDist + PosY)), CurLoc.Z);
	//}
}

float APPuppyCharacter::WrapToNavRange(const float Loc)
{
	float ReturnValue;

	if (Loc < NavRangeMin) {
		ReturnValue =  NavRangeMax - (NavRangeMin - Loc);
	}
	else if (Loc > NavRangeMax) {
		ReturnValue =  NavRangeMin + (NavRangeMax - Loc);
	}
	else { ReturnValue = Loc; }

	if (FMath::Abs(ReturnValue - GetActorLocation().Y) < 100) {
		UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f, %f, %f, %f"), NavRangeMin, NavRangeMax, NavMinMoveDist, Loc, GetActorLocation().Y, ReturnValue);
	}

	return ReturnValue;
}

void APPuppyCharacter::OnIncreaseDifficulty(float NavMinMoveDistIn, float SpeedIn, float AccelIn)
{
	NavMinMoveDist = NavMinMoveDistIn;
	
	if (MovementComponent) {
		if(AccelIn != 0.0f) MovementComponent->MaxAcceleration = AccelIn;
		if(SpeedIn != 0.0f) MovementComponent->MaxWalkSpeed = SpeedIn;
	}
}
