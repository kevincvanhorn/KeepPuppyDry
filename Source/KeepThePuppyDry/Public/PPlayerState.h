// Copyright 2020, Kevin VanHorn. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PCustomizationManager.h"
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

	//void SetPPlayer(class APPlayer* PPlayerIn);

	void ResetLocalGameSave();

public:
	bool GetTutorialEnabled() const { return bShowTutorial; }

	const TArray<uint8>& GetUmbrellaPatterns() const { return UmbrellaPatterns; }

	void IncreaseScoreRate(float PosRate, float NegRate); // TODO: rename

// PERSISTENT DATA: Saved locally
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		int32 PScore; // Bones = Currency // TODO: rename

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bShowTutorial;
	
public:
	void SetShowTutorial(bool bVisible) { bShowTutorial = bVisible; }

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Score")
		float ScoreMultiplier;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Health")
		float HPositiveRate;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Health")
		float HNegativeRate;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		int32 TriesBetweenInterstitialAds;

	UFUNCTION(BlueprintCallable)
		bool TryDisplayInterstitialAd();

	void ShowAdBanner();

	void HideAdBanner();

	int32 GetSessionScore();

public:
	UFUNCTION(BlueprintCallable)
		int32 ScoreFromTime(float TotalTime);

	UFUNCTION(BlueprintCallable)
		float GetHealth() const; // [0-1]

	/** Returns the "score" TODO: should be renamed currency. */
	UFUNCTION(BlueprintCallable)
		int32 GetScore() const;

	UFUNCTION(BlueprintCallable)
		int32 GetNumSessionLosses() const;

	UFUNCTION(BlueprintCallable)
		float ChangeHealth(float Delta);

	UFUNCTION(BlueprintCallable)
		float AddHealth(float DeltaTime);

	UFUNCTION(BlueprintCallable)
		float SubtractHealth(float DeltaTime);

	UFUNCTION(BlueprintCallable)
		void IncrementScore();

	void SetScore(int32 ScoreIn);

	void SetPUserWidget(class UPUserWidget* PUserWidgetIn);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		float Health;

	void GameOver();

private:
	class UPUserWidget* PUserWidget;

	class UPGameInstance* PGameInstance;

	float UnclampedHealth;
	
	bool bIsGameOver;

	//int32 NumSessionLosses;

	bool bShowAdOnNextRequest;

// Customization:
public:
	bool PlayerOwnsAsset(EUmbrellaPattern Pattern);

	bool BuyAsset(EUmbrellaPattern UmbrellaPattern, int32 Cost);

	bool bCanAffordAsset(int32 Cost);

	void SetLastSelected(EUmbrellaPattern UmbrellaPattern);

	int32 LastSelected_UmbrellaPattern;

protected:
	TArray<uint8> UmbrellaPatterns;

	FString PLATFORM;

	UFUNCTION()
		void OnGameStart();

	int32 SessionStartScore;
};
