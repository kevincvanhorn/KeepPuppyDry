// Fill out your copyright notice in the Description page of Project Settings.


#include "PHumanSpawner.h"
#include "Engine/World.h"
#include "PHuman.h"

// Sets default values
APHumanSpawner::APHumanSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurSpawnIndex = 0;
	SpawnRate = 1;
}

// Called when the game starts or when spawned
void APHumanSpawner::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &APHumanSpawner::SpawnHuman, SpawnRate, false);
}

void APHumanSpawner::SpawnHuman()
{
	if (SpawnLocs.Num() <= 0) return;

	if (CurSpawnIndex >= SpawnLocs.Num()) {
		CurSpawnIndex = 0;
	}

	if (HumanClass) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		APHuman* Human = (APHuman*)GetWorld()->SpawnActor<APHuman>(HumanClass, SpawnLocs[CurSpawnIndex], FRotator(0, 0, 0), SpawnParams);
		if (Human) {
			Human->SetActorEnableCollision(false);
		}
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &APHumanSpawner::SpawnHuman, SpawnRate, false);
	}
}

// Called every frame
void APHumanSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

