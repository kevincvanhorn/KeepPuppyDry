// Copyright 2019, Kevin VanHorn. All rights reserved.


#include "PAudioManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APAudioManager::APAudioManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APAudioManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APAudioManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

