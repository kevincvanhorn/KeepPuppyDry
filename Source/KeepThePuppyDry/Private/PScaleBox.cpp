// Copyright 2020, Kevin VanHorn. All rights reserved.


#include "PScaleBox.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"

UPScaleBox::UPScaleBox() {
}

void UPScaleBox::MoveTo(FVector2D StartLoc, FVector2D TargetLocIn, float InterpSpeed) {
	MoveSpeed = InterpSpeed;
	FVector2D TargetLoc;
	
	this->RenderTransform.Translation = StartLoc;

	UWorld* World = GetWorld();
	if (World) {
		FTimerManager& WorldTimeManager = World->GetTimerManager();
		WorldTimeManager.SetTimer(MoveHandle, this, &UPScaleBox::UpdateLocation, 0.01f, true, 0.0f);
	}
}

void UPScaleBox::UpdateLocation() {
	FVector2D CurLoc = this->RenderTransform.Translation;

	if (FVector2D::Distance(CurLoc, TargetLoc) > 0.001f) {
		this->RenderTransform.Translation = UKismetMathLibrary::Vector2DInterpTo(CurLoc, TargetLoc, 0.01f, MoveSpeed);
	}
	else{
		UWorld* World = GetWorld();
		if (World) {
			FTimerManager& WorldTimeManager = World->GetTimerManager();
			WorldTimeManager.ClearTimer(MoveHandle);
		}
	}

}