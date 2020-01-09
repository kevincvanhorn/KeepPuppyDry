// Copyright 2020, Kevin VanHorn. All rights reserved.

#include "PAIController.h"
#include "PPuppyCharacter.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "AI/NavigationSystemBase.h"
#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "PSwipeDelegates.h"
#include "PLevelScriptActor.h"

APAIController::APAIController() {
	ActionDelay = 5.0f;
	TimeSinceLastAction = 0;
	SitProbability = 0.3f;
	WaitProbability = 0.2f;
	Difficulty = 0;
}

void APAIController::BeginPlay()
{
	Super::BeginPlay();

	Level = (APLevelScriptActor*)GetWorld()->GetLevelScriptActor();

	Puppy = (APPuppyCharacter*)GetPawn();
	Navigation = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	if (Level) {
		Level->SetAIController(this);
	}

	// Start recursive action timer:
	DoNextAction();
}
void APAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimeSinceLastAction += DeltaTime;
}

void APAIController::DoNextAction()
{
	bool bCanRequestMove = true; 
	bool bMovedThisAction = false;
	EPathFollowingStatus::Type MoveStatus = GetMoveStatus();

	// Determine Action:
	bool bWait = UKismetMathLibrary::RandomBoolWithWeight(WaitProbability);
	bool bSit = UKismetMathLibrary::RandomBoolWithWeight(SitProbability); // Note: this is chained probability.

	// Wait Action:
	if (bWait) {
		// Do nothing.
	}
	// Sit Action:
	else if (bSit) {
		UPSwipeDelegates::PuppySitDelegate.Broadcast(); // Update Animation
	}
	// Move Action:
	else if (Puppy && Navigation) {
		UPSwipeDelegates::PuppyStandDelegate.Broadcast(); // Update Animation
		FNavLocation  ProjectedPoint;
		bCanRequestMove = Navigation->ProjectPointToNavigation(Puppy->GetNextRandomLocation(), ProjectedPoint);
		if (bCanRequestMove) {
			bMovedThisAction = true;
			MoveToLocation(ProjectedPoint.Location);
		}
	}

	// Recurse to next action:
	if (!bMovedThisAction) {
		GetWorldTimerManager().SetTimer(AITimerHandle, this, &APAIController::DoNextAction, ActionDelay, false);
	}

	TimeSinceLastAction = 0;
}

void APAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	// Notify blueprint via event:
	Super::OnMoveCompleted(RequestID, Result);

	// Call next action:
	float RemainingDelay = FMath::Clamp(ActionDelay - TimeSinceLastAction, 0.0f, ActionDelay);
	if (RemainingDelay == 0) {
		DoNextAction();
	}
	else {
		GetWorldTimerManager().SetTimer(AITimerHandle, this, &APAIController::DoNextAction, RemainingDelay, false);
	}
}

bool APAIController::IncreaseDifficulty()
{
	if (Puppy && Difficulty < DifficultyLevels.Num()) {
		FAIDIfficultyStruct State = DifficultyLevels[Difficulty];
		ActionDelay = State.ActionDelay;
		WaitProbability = State.WaitProbability;
		SitProbability = State.SitProbability;

		Puppy->OnIncreaseDifficulty(State.NavMinMoveDist, State.PSpeed, State.PAccel);

		Difficulty++;
		return true;
	}
	return false;
}
