// Copyright 2020, Kevin VanHorn. All rights reserved.


#include "PHuman.h"

// Sets default values
APHuman::APHuman()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Lifetime = 2;
	TimeAlive = 0;
	Speed = 1;
}

void APHuman::OverrideDirection(FVector DirectionIn)
{
	Direction = DirectionIn;
}

void APHuman::OverrideSpeed(float SpeedIn)
{
	Speed = SpeedIn;
}

// Called when the game starts or when spawned
void APHuman::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APHuman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + Direction * Speed*DeltaTime);
	TimeAlive += DeltaTime;
	if (TimeAlive > Lifetime) {
		Destroy();
	}
}

