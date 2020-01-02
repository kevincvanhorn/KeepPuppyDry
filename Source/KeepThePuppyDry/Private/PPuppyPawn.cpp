// Copyright 2020, Kevin VanHorn. All rights reserved.

#include "PPuppyPawn.h"
#include "Components/SkeletalMeshComponent.h"

APPuppyPawn::APPuppyPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	if (SkeletalMeshComp) {
		SkeletalMeshComp->SetupAttachment(RootComponent);
	}
}

void APPuppyPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APPuppyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APPuppyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


