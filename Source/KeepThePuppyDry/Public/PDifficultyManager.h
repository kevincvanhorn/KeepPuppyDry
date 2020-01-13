// Copyright 2020, Kevin VanHorn. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PDifficultyManager.generated.h"

UCLASS()
class KEEPTHEPUPPYDRY_API APDifficultyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	APDifficultyManager();

	void SetPUserWidget(class UPUserWidget* PUserWidgetIn);

protected:
	virtual void BeginPlay() override;

	class UPUserWidget* PUserWidget;

public:	
	virtual void Tick(float DeltaTime) override;

	/** Start the difficulty loop tick. This should only be called once on level begin - Use Unpause for resume behaviour. */
	UFUNCTION(BlueprintCallable)
		void Start();

	/** Pause the difficulty loop tick. */
	UFUNCTION(BlueprintCallable)
		void Pause();

	/** UnPause the difficulty loop tick. */
	UFUNCTION(BlueprintCallable)
		void UnPause();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		int32 Difficulty;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnSwitchDifficulty"))
		void OnSwitchDifficulty(int32 DifficultyOut);

protected:
	UFUNCTION()
		void IncreaseDifficulty();

	/** The time (sec) it takes to get to an indexed difficulty relative to the previous. ex: [0] = 0 sec, [1] = 10 sec, [2] = 10 sec. */
	// Dev note: Do NOT edit this at runtime.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TArray<float> TimeToDifficulties;

	int32 MaxDifficulty;

	FTimerHandle DifficultyHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 UIFramesToSkip;

private:
	bool bGameLoopActive;

	int32 CurUIFrame;
};
