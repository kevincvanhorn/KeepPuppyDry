// Copyright 2019, Kevin VanHorn. All rights reserved.


#include "PPlayerState.h"

APPlayerState::APPlayerState() {
	ScoreMultiplier = 1.0f;
}

int32 APPlayerState::ScoreFromTime(float TotalTime)
{
	return TotalTime * ScoreMultiplier;
}

void APPlayerState::BeginPlay()
{
	Super::BeginPlay();
}
