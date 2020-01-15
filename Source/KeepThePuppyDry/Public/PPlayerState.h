// Copyright 2020, Kevin VanHorn. All rights reserved.

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

	bool LoadGame();

	bool SaveGame();

	void SetPPlayer(class APPlayer* PPlayerIn);

public:
	bool GetTutorialEnabled() const { return bShowTutorial; }

// PERSISTENT DATA: Saved locally
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		int32 PScore; // Bones = Currency // TODO: rename

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		bool bShowTutorial;
	
public:

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

	/** Returns the "score" TODO: should be renamed currency. */
	UFUNCTION(BlueprintCallable)
		int32 GetScore() const;

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

	void GameOver();

private:
	class UPUserWidget* PUserWidget;

	float UnclampedHealth;
	
	bool bIsGameOver;
};
