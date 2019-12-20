// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PHumanSpawner.generated.h"

UCLASS()
class KEEPTHEPUPPYDRY_API APHumanSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APHumanSpawner();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TArray<FVector> SpawnLocs;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float SpawnRate;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TSubclassOf<class APHuman> HumanClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void SpawnHuman();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FTimerHandle SpawnTimerHandle;

	int CurSpawnIndex;

};
