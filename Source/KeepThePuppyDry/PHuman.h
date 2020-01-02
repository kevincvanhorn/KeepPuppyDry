// Copyright 2020, Kevin VanHorn. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PHuman.generated.h"

UCLASS()
class KEEPTHEPUPPYDRY_API APHuman : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APHuman();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Lifetime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector Direction;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Speed;

	void OverrideDirection(FVector DirectionIn);

	void OverrideSpeed(float SpeedIn);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	float TimeAlive;

};
