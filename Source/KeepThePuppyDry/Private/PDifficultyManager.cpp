// Copyright 2019, Kevin VanHorn. All rights reserved.


#include "PDifficultyManager.h"
#include "PSwipeDelegates.h"
#include "TimerManager.h"
#include "PUserWidget.h"

APDifficultyManager::APDifficultyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bGameLoopActive = false;
	Difficulty = 0;	
	MaxDifficulty = 0;
	CurUIFrame = 0;
	UIFramesToSkip = 4;
}

void APDifficultyManager::SetPUserWidget(class UPUserWidget* PUserWidgetIn)
{
	PUserWidget = PUserWidgetIn;
}

void APDifficultyManager::BeginPlay()
{
	Super::BeginPlay();
}

void APDifficultyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (++CurUIFrame > UIFramesToSkip && PUserWidget) {
		PUserWidget->SetDTimeRemaining(GetWorldTimerManager().GetTimerRemaining(DifficultyHandle));
		CurUIFrame = 0;
	}
}

void APDifficultyManager::Start()
{
	MaxDifficulty = FMath::Clamp(TimeToDifficulties.Num() - 1, 0, 10000); // 10000 is arbitrary
	bGameLoopActive = true;
	if (MaxDifficulty > 0 && Difficulty < MaxDifficulty && MaxDifficulty == TimeToDifficulties.Num() - 1) {
		GetWorldTimerManager().SetTimer(DifficultyHandle, this, &APDifficultyManager::IncreaseDifficulty, TimeToDifficulties[Difficulty+1], false);
	}
}

void APDifficultyManager::IncreaseDifficulty()
{
	this->OnSwitchDifficulty(++Difficulty);
	UPSwipeDelegates::DifficultySwitchDelegate.Broadcast(Difficulty);

	if (Difficulty < MaxDifficulty) {
		GetWorldTimerManager().SetTimer(DifficultyHandle, this, &APDifficultyManager::IncreaseDifficulty, TimeToDifficulties[Difficulty + 1], false);
	}
}

void APDifficultyManager::Pause()
{
	GetWorldTimerManager().PauseTimer(DifficultyHandle);
	bGameLoopActive = false;
}

void APDifficultyManager::UnPause()
{
	GetWorldTimerManager().UnPauseTimer(DifficultyHandle);
	bGameLoopActive = true;
}

