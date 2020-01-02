// Copyright 2020, Kevin VanHorn. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PPuppyPawn.generated.h"

UCLASS()
class KEEPTHEPUPPYDRY_API APPuppyPawn : public APawn
{
	GENERATED_BODY()

public:
	APPuppyPawn();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class USkeletalMeshComponent* SkeletalMeshComp;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};


