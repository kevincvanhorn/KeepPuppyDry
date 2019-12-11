// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PPlayer.generated.h"

UCLASS()
class KEEPTHEPUPPYDRY_API APPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APPlayer();

	class USpringArmComponent* GetSpringArmComponent() const { return SpringArm; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
		class UCameraComponent* Camera;

	class APPlayerController* PPlayerController;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnTouchMoved(const FVector2D& DeltaTouch);

	void UpdateTouchLoc(FVector2D _TouchLoc);

	UFUNCTION()
		void OnSwipeUp();

	UFUNCTION()
		void OnSwipeDown();

	UFUNCTION()
		void OnDoubleTap();

	UFUNCTION()
		void Zoom(float AxisValue);

private:
	FVector StartLoc;

	FVector2D TouchLoc;
};
