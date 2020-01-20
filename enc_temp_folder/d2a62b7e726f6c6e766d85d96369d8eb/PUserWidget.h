// Copyright 2020, Kevin VanHorn. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PLevelScriptActor.h"
#include "PGameInstance.h"
#include "PUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class KEEPTHEPUPPYDRY_API UPUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetTouchDragPosition(FVector2D TouchPos);

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "DebugTouchPosition"))
		void DebugTouchPosition(FVector2D TouchPos);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnUpdateUIDifficulty"))
		void OnUpdateUIDifficulty(EDifficultyType Difficulty);

	void UpdateScore(int32 ScoreIn);

	void UpdateHealth(float HealthIn);

	void SetHealthOver(float HealthOver);

	void UpdateUIDifficulty(EDifficultyType DiffType);

	void SetDTimeRemaining(float TimeRemaining);

protected:
	class UPGameInstance* PGameInstance;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UScaleBox* HandWidget;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* ScoreTextWidget;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* DTimerWidget;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UMaterialParameterCollection* MPC_Score;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		float TimeToNextDifficulty;


	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnUpdateScore"))
		void OnUpdateScore();

// Game Over
public:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnGameOver"))
		void OnGameOver();

private:
	UFUNCTION()
		void OnGameOverInternal();

// SOUNDS
protected:
	UFUNCTION(BlueprintCallable)
		void PlaySound2D(class USoundBase* Sound, ESoundType SoundType, float VolumeMultiplier = 1.0f, float PitchMultiplier = 1.0f, float StartTime = 0.0f);

	UFUNCTION(BlueprintCallable)
		void PlayPersisentSound(ESoundLabels SoundLabel, float FadeInDuration);

	UFUNCTION(BlueprintCallable)
		void StopPersistentSound(ESoundLabels SoundLabel, float FadeOutDuration);
};