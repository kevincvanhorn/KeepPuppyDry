// Fill out your copyright notice in the Description page of Project Settings.


#include "PAIController.h"

APAIController::APAIController() {

}

FVector APAIController::GetRandomWaypoint()
{
	if (Waypoints.Num() > 0) {
		int index = FMath::RandRange(0, Waypoints.Num() - 1);
		return Waypoints[index];
	}
	else if (GetPawn()) {
		return GetPawn()->GetActorLocation();
	}
	else return FVector::ZeroVector;

}

void APAIController::GoToRandomWaypoint()
{
	MoveToLocation(GetRandomWaypoint());
}
