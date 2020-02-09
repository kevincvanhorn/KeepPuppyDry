// Copyright 2020, Kevin VanHorn. All rights reserved.


#include "PEdgeCollider.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"	
#include "PPuppyCharacter.h"
#include "PPlayer.h"

// Sets default values
APEdgeCollider::APEdgeCollider()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	if (BoxComponent) {
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APEdgeCollider::OnOverlapBegin);
		BoxComponent->OnComponentEndOverlap.AddDynamic(this, &APEdgeCollider::OnOverlapEnd);
	}

}

// Called when the game starts or when spawned
void APEdgeCollider::BeginPlay()
{
	Super::BeginPlay();

	PPlayer = (APPlayer*) UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
}

void APEdgeCollider::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APPuppyCharacter* Puppy = dynamic_cast<APPuppyCharacter*>(OtherActor);
	if (PPlayer && Puppy) {
		PPlayer->ZoomOut();
	}
}

void APEdgeCollider::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APPuppyCharacter* Puppy = dynamic_cast<APPuppyCharacter*>(OtherActor);
	if (PPlayer && Puppy) {
		PPlayer->ZoomIn();
	}
}
