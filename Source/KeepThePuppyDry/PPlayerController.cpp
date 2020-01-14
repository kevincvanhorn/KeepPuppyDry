// Copyright 2020, Kevin VanHorn. All rights reserved.


#include "PPlayerController.h"
#include "Public/TimerManager.h"
#include "PSwipeDelegates.h"
#include "PPlayer.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "Public/PUserWidget.h"
#include "KeepThePuppyDry.h"
#include "PPlayerState.h"
#include "PDifficultyManager.h"
#include "PLevelScriptActor.h"
#include "PMainMenuWidget.h"

#include "Kismet/GameplayStatics.h"

APPlayerController::APPlayerController() {
	bIgnoreInput = false;

	MinSwipeDistance = 1.0f;
	TapCount = 0;
	MaxDoubleTapDelay = 0.5f;
}


void APPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APPlayerController::BeginPlay() {
	PPlayer = Cast<APPlayer>(GetPawn());
	PPlayerState = Cast<APPlayerState>(PlayerState);
	PLevel = (APLevelScriptActor*)GetWorld()->GetLevelScriptActor();

	UPSwipeDelegates::GameOverDelegate.AddUObject(this, &APPlayerController::OnGameOver);

	// Start Screen (load this first).
	if (PMainMenuWidgetClass) {
		PMainMenuWidget = CreateWidget<UPMainMenuWidget>(GetWorld(), PMainMenuWidgetClass);
		if (PMainMenuWidget) {
			PMainMenuWidget->Initialize(this);
			PMainMenuWidget->AddToViewport();
		}
	}

	// Spawn Managers & Pre-StartGame initialization:
	if (PUserWidgetClass) {
		PUserWidget = CreateWidget<UPUserWidget>(GetWorld(), PUserWidgetClass);
		if (PUserWidget) {
			if (PPlayerState) {
				PPlayerState->SetPUserWidget(PUserWidget);
			}
			if (PLevel) {
				PLevel->SetUserWidget(PUserWidget);
			}
		}
	}
	
	if (PPlayer) {
		PPlayer->Initialize(PPlayerState);
		PPlayer->SetUserWidget(PUserWidget); // Sets to nullptr if UserWidget does not exist.
	}
	if (DifficultyManagerClass) {
		DifficultyManager = GetWorld()->SpawnActor<APDifficultyManager>(DifficultyManagerClass);
		DifficultyManager->SetPUserWidget(PUserWidget);
	}

	// Main Menu:
	UGameplayStatics::SetGamePaused(GetWorld(), true); // Pause game.
	if (PPlayerState) {
		PPlayerState->LoadGame();
	}

	UPSwipeDelegates::PostBeginPlayDelegate.Broadcast();
}

void APPlayerController::StartGame()
{
	if (PUserWidget) {
		PUserWidget->AddToViewport();
	}

	/*if (PMainMenuWidget) {
		PMainMenuWidget->RemoveFromViewport();
	}*/
	UGameplayStatics::SetGamePaused(GetWorld(), false); // UnPause game.
}

void APPlayerController::OnGameOver()
{
	/*if (UserWidget) {
		UserWidget->RemoveFromViewport();
	}*/
}

void APPlayerController::ResetTapHandler()
{
	TapCount = 0;
}

bool APPlayerController::InputTouch(uint32 Handle, ETouchType::Type Type, const FVector2D& TouchLocation, float Force, FDateTime DeviceTimestamp, uint32 TouchpadIndex)
{
	Super::InputTouch(Handle, Type, TouchLocation, Force, DeviceTimestamp, TouchpadIndex);

	if (bIgnoreInput) return false;


	if (Type == ETouchType::Began) {
		SwipeStartLoc = TouchLocation;
		LastTouchLoc = SwipeStartLoc;

		bIsSwiping = true;
		SwipeDirection = Swipe::Direction::None;

		/*if (PPlayer) {
			PPlayer->UpdateTouchLoc(TouchLocation);
		}*/

		UPSwipeDelegates::TouchBeganDelegate.Broadcast();
	}
	else if (Type == ETouchType::Moved && bIsSwiping) {
		FVector2D DeltaLoc = TouchLocation - LastTouchLoc; // Change in distance

		if (bIsSwiping && SwipeDirection == Swipe::Direction::None) {
			float AbsX = FMath::Abs(DeltaLoc.X);
			float AbsY = FMath::Abs(DeltaLoc.Y);
			bool bXThreshold = (AbsX >= MinSwipeDistance);
			bool bYThreshold = (AbsY >= MinSwipeDistance);

			if (AbsX > AbsY && bXThreshold) {
				if (DeltaLoc.X > 0) {
					UPSwipeDelegates::SwipeRightDelegate.Broadcast();
					SwipeDirection = Swipe::Direction::Right;
				}
				else {
					UPSwipeDelegates::SwipeLeftDelegate.Broadcast();
					SwipeDirection = Swipe::Direction::Left;
				}
			}
			else if (bYThreshold) {
				if (DeltaLoc.Y > 0) {
					UPSwipeDelegates::SwipeDownDelegate.Broadcast();
					SwipeDirection = Swipe::Direction::Down;
				}
				else {
					UPSwipeDelegates::SwipeUpDelegate.Broadcast();
					SwipeDirection = Swipe::Direction::Up;
				}
			}
		}

		if (PPlayer) {
			//UE_LOG(LogTemp, Warning, TEXT("Touch: Moved"));
			PPlayer->OnTouchMoved(DeltaLoc);
			PPlayer->UpdateTouchLoc(TouchLocation);
		}

		LastTouchLoc = TouchLocation;
	}
	else if (Type == ETouchType::Ended) {
		//UE_LOG(LogTemp, Warning, TEXT("Touch: Ended"));
		bIsSwiping = false;

		if (SwipeDirection == Swipe::Direction::None) {
			TapCount++;
		}

		if (TapCount == 2) {
			UPSwipeDelegates::DoubleTapDelegate.Broadcast();
		}
		FTimerHandle TapHandler;
		FTimerDelegate TapHandlerDelegate;
		TapHandlerDelegate.BindUObject(this, &APPlayerController::ResetTapHandler);
		UWorld* World = GetWorld();
		if (World)
		{
			World->GetTimerManager().SetTimer(TapHandler, TapHandlerDelegate, MaxDoubleTapDelay, false);
		}

		UPSwipeDelegates::TouchEndedDelegate.Broadcast();
	}

	return false;
}

void APPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}
