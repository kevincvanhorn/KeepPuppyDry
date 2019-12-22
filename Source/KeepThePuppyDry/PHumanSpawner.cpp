#include "PHumanSpawner.h"
#include "Engine/World.h"
#include "PHuman.h"
#include "TimerManager.h"

// Sets default values
APHumanSpawner::APHumanSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurSpawnIndex = 0;
	SpawnRate = 1;

	OverrideSpeed = 0;
	OverrideDirection = FVector::ZeroVector;
	bDifferentFirstSpawnLoc = false;
}

// Called when the game starts or when spawned
void APHumanSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (bDifferentFirstSpawnLoc) CurSpawnIndex = -1;

	SpawnHuman();
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

		if (bDifferentFirstSpawnLoc) {
			if (CurSpawnIndex == 0) {
				CurSpawnIndex++;
				if (SpawnLocs.Num() <= 1) return;
			}
			else if (CurSpawnIndex == -1) CurSpawnIndex++;
		}

		APHuman* Human = (APHuman*)GetWorld()->SpawnActor<APHuman>(HumanClass, SpawnLocs[CurSpawnIndex], FRotator(0, 0, 0), SpawnParams);
		if (Human) {
			Human->SetActorEnableCollision(false);

			if (OverrideSpeed != 0) {
				Human->OverrideSpeed(OverrideSpeed);
			}
			if (OverrideDirection != FVector::ZeroVector) {
				Human->OverrideDirection(OverrideDirection);
			}
		}
		// Repeat spawn timer
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &APHumanSpawner::SpawnHuman, SpawnRate, false);
	}
}

// Called every frame
void APHumanSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

