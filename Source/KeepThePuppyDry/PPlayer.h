// Copyright 2020, Kevin VanHorn. All rights reserved.

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

	void SetUserWidget(class UPUserWidget* WidgetIn) { PUserWidget = WidgetIn; }

	class USpringArmComponent* GetSpringArmComponent() const { return SpringArm; }

	void Initialize(class APPlayerState* PlayerStateIn);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void PostBeginPlay();

	UFUNCTION()
		void OnDifficultyChangedInternal(int32 NewDifficulty);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnDifficultyChanged"))
		void OnDifficultyChanged(int32 DifficultyOut);

	UPROPERTY(EditAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
		class UCameraComponent* Camera;

	class APPlayerController* PPlayerController;

	class UPUserWidget* PUserWidget;
	
	class APPlayerState* PPlayerState;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnTouchMoved(const FVector2D& DeltaTouch);

	void UpdateTouchLoc(FVector2D TouchLocIn);

	UFUNCTION()
		void OnSwipeUp();

	UFUNCTION()
		void OnSwipeDown();

	UFUNCTION()
		void OnDoubleTap();

	UFUNCTION()
		void OnTouchBegin();

	UFUNCTION()
		void OnTouchEnd();

	UFUNCTION()
		void Zoom(float AxisValue);

private:
	FVector StartLoc;

	FVector2D TouchLoc;

	bool bScreenTouched;

// Umbrella
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Umbrella")
		TSubclassOf<class APUmbrella> UmbrellaClass;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Umbrella")
		FVector UmbrellaSpawnLoc;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Umbrella")
		FVector UTouchPosition;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Umbrella")
		FVector UReleasePosition;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Umbrella")
		float ClampZPos;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Umbrella")
		float ClampZNeg;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Umbrella")
		class UMaterialParameterCollection* MPC_Umbrella;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnUTouchBegin"))
		void OnUTouchBegin();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnUTouchEnd"))
		void OnUTouchEnd();

private:
	class APUmbrella* Umbrella;


// SCORES ----------------------------
public:
	void OnUmbrellaOverlapBegin();
	void OnUmbrellaOverlapEnd();

private:
	float UOverlapTime; // How long overlapping, 0 if not currently overlapping
	bool bUOverlapping; // Overlapping umbrella?

	class UMaterialParameterCollection* MPC;
};
