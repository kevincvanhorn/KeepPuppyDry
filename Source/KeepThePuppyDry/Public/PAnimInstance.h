// Copyright 2020, Kevin VanHorn. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class KEEPTHEPUPPYDRY_API UPAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPAnimInstance();

public:
	APawn* OwningPawn;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class APPuppyCharacter* Puppy;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bSitting;

	/** [0-1] Speed representation based on character max move speed.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Speed;

	UFUNCTION()
		void OnPuppySit();

	UFUNCTION()
		void OnPuppyStand();

	UFUNCTION()
		void PostBeginPlay();

private:
	float MaxMoveSpeed;
};
