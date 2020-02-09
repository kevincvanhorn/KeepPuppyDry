// Copyright 2020, Kevin VanHorn. All rights reserved.


#include "PScaleBox.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"

UPScaleBox::UPScaleBox() {
}

void UPScaleBox::MoveTo(FVector2D StartLocIn, FVector2D TargetLocIn, float InterpSpeed) {
	this->SetVisibility(ESlateVisibility::Visible);

	MoveSpeed = InterpSpeed;
	StartLoc = StartLocIn;
	TargetLoc = TargetLocIn;

	this->SetRenderTranslation(StartLoc);
	this->SetRenderOpacity(1.0f);

	TotalDistInv = 1.0f / FVector2D::Distance(StartLoc, TargetLoc);

	UWorld* World = GetWorld();
	if (World) {
		FTimerManager& WorldTimeManager = World->GetTimerManager();
		WorldTimeManager.ClearAllTimersForObject(this);
		WorldTimeManager.SetTimer(MoveHandle, this, &UPScaleBox::UpdateLocation, 0.01f, true, 0.0f);
	}
}

void UPScaleBox::UpdateLocation() {
	FVector2D CurLoc = this->RenderTransform.Translation;
	float Dist = FVector2D::Distance(CurLoc, TargetLoc);
	
	if (Dist > 0.001f) {
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *TargetLoc.ToString());
		this->SetRenderTranslation(UKismetMathLibrary::Vector2DInterpTo(CurLoc, TargetLoc, 0.01f, MoveSpeed));
		this->SetRenderOpacity(Dist*TotalDistInv);
	}
	else{
		UWorld* World = GetWorld();
		if (World) {
			FTimerManager& WorldTimeManager = World->GetTimerManager();
			WorldTimeManager.ClearTimer(MoveHandle);
		}
	}
}