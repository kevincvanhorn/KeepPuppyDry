// Copyright 2020, Kevin VanHorn. All rights reserved.

#include "PPuppyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "PSwipeDelegates.h"

// Sets default values
APPuppyCharacter::APPuppyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Navigation
	NavMinMoveDist = 100;
	NavRadius = 500;
	NavRangeMin = NavOrigin.Y - NavRadius;
	NavRangeMax = NavOrigin.Y + NavRadius;
}

// Called when the game starts or when spawned
void APPuppyCharacter::BeginPlay()
{
	Super::BeginPlay();

	UPSwipeDelegates::DifficultySwitchDelegate.AddUniqueDynamic(this, &APPuppyCharacter::OnDifficultyChangedInternal);
}

void APPuppyCharacter::OnDifficultyChangedInternal(int32 NewDifficulty)
{
	this->OnDifficultyChanged(NewDifficulty);
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
	float PosY = UKismetMathLibrary::RandomFloatInRange(NavRangeMin, NavRangeMax);
	FVector CurLoc = GetActorLocation();

	/*if (FMath::Abs(CurLoc.Y - PosY) < NavMinMoveDist) {
		return FVector(CurLoc.X, PosY, CurLoc.Z);
	}
	else {*/
		int Dir = (int)UKismetMathLibrary::RandomBool();
		return FVector(CurLoc.X, WrapToNavRange(Dir * NavMinMoveDist + PosY), CurLoc.Z);
	//}
}

float APPuppyCharacter::WrapToNavRange(const float Loc)
{
	float ReturnValue;

	if (Loc < NavRangeMin) {
		ReturnValue =  NavRangeMax - (NavRangeMin - Loc);
	}
	else if (Loc > NavRangeMax) {
		ReturnValue =  NavRangeMin - (NavRangeMax - Loc);
	}
	else { ReturnValue = Loc; }

	if (FMath::Abs(ReturnValue - GetActorLocation().Y) < NavMinMoveDist) {
		int i = 0;
	}

	return ReturnValue;
}
