// Copyright 2020, Kevin VanHorn. All rights reserved.


#include "PAnimInstance.h"
#include "PPuppyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PSwipeDelegates.h"

UPAnimInstance::UPAnimInstance() {
	bSitting = false;
	Speed = 0;
	MaxMoveSpeed = 0;
}

void UPAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningPawn = TryGetPawnOwner();
	Puppy = (APPuppyCharacter*)OwningPawn;

	UPSwipeDelegates::PuppySitDelegate.AddUObject(this, &UPAnimInstance::OnPuppySit);
	UPSwipeDelegates::PuppyStandDelegate.AddUObject(this, &UPAnimInstance::OnPuppyStand);
	UPSwipeDelegates::PostBeginPlayDelegate.AddUObject(this, &UPAnimInstance::PostBeginPlay);
}

void UPAnimInstance::PostBeginPlay()
{
	if (Puppy) {
		UCharacterMovementComponent* MoveComp = Puppy->GetCharacterMovement();
		if (MoveComp) { MaxMoveSpeed = MoveComp->GetMaxSpeed(); }
	}
}

void UPAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	Super::NativeUpdateAnimation(DeltaTimeX);

	if (Puppy) {
		// Move if not sitting:
		if (!bSitting) {
			float fVelocity = Puppy->GetVelocity().Size();

			if (fVelocity <= 0) {
				Speed = 0;
			}
			else {
				Speed = MaxMoveSpeed / fVelocity;
			}
		}
	}
}

void UPAnimInstance::OnPuppySit()
{
	bSitting = true;
	Speed = 0;
}


void UPAnimInstance::OnPuppyStand()
{
	bSitting = false;
}
