// Copyright 2019, Kevin VanHorn. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class KEEPTHEPUPPYDRY_API APPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	APPlayerState();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Score")
		float ScoreMultiplier;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Health")
		float HPositiveRate;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Health")
		float HNegativeRate;

public:
	UFUNCTION(BlueprintCallable)
		int32 ScoreFromTime(float TotalTime);

	UFUNCTION(BlueprintCallable)
		float GetHealth() const; // [0-1]

	UFUNCTION(BlueprintCallable)
		float ChangeHealth(float Delta);

	UFUNCTION(BlueprintCallable)
		float AddHealth(float DeltaTime);

	UFUNCTION(BlueprintCallable)
		float SubtractHealth(float DeltaTime);

	UFUNCTION(BlueprintCallable)
		void IncrementScore();

	void SetPUserWidget(class UPUserWidget* PUserWidgetIn);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		float Health;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		int32 PScore;

private:
	class UPUserWidget* PUserWidget;

	float UnclampedHealth;
};
