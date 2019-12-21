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

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FVector> SpawnLocs;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float SpawnRate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<class APHuman> HumanClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float OverrideSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector OverrideDirection;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bDifferentFirstSpawnLoc; // for starting an initial spawn in the beginning of the screen.

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
