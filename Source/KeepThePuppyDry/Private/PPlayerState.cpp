// Copyright 2019, Kevin VanHorn. All rights reserved.


#include "PPlayerState.h"
#include "PUserWidget.h"

APPlayerState::APPlayerState() {
	ScoreMultiplier = 1.0f;
	Health = 1.0f;
	HPositiveRate = HNegativeRate = 1.0f;
	UnclampedHealth = 1.0f;
}

int32 APPlayerState::ScoreFromTime(float TotalTime)
{
	return TotalTime * ScoreMultiplier;
}

float APPlayerState::GetHealth() const
{
	return Health;
}

float APPlayerState::ChangeHealth(float Delta)
{
	return Delta >= 0 ? AddHealth(Delta) : SubtractHealth(-1*Delta);
}

// Supports adding negative
float APPlayerState::AddHealth(float DeltaTime)
{
	// UnclampedHealth can go under zero, but not gameplay relevant.
	UnclampedHealth = (UnclampedHealth + DeltaTime * HPositiveRate);
	if (UnclampedHealth >= 2) {
		UnclampedHealth  = FMath::Clamp(UnclampedHealth, 0.0f, 1.0f);
		IncrementScore();
	}
	Health = FMath::Clamp(UnclampedHealth, 0.0f, 1.0f);
	if (PUserWidget) {
		PUserWidget->SetHealthOver(FMath::Clamp(UnclampedHealth - 1.0f, 0.0f,1.0f));
	}
	return Health;
}

float APPlayerState::SubtractHealth(float DeltaTime)
{
	UnclampedHealth = FMath::Clamp(UnclampedHealth, 0.0f, 1.0f); // Reset to 1 if above when subtracting
	UnclampedHealth = (UnclampedHealth + DeltaTime * HNegativeRate * -1);
	Health = FMath::Clamp(UnclampedHealth, 0.0f, 1.0f);
	if (PUserWidget) {
		PUserWidget->SetHealthOver(0.0f);
	}
	return Health;
}

void APPlayerState::IncrementScore()
{
	PScore++;
	if (PUserWidget) {
		PUserWidget->UpdateScore(PScore);
	}
}

void APPlayerState::SetPUserWidget(UPUserWidget* PUserWidgetIn)
{
	PUserWidget = PUserWidgetIn;
}

void APPlayerState::BeginPlay()
{
	Super::BeginPlay();
}
