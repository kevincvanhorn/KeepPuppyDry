// Copyright 2020, Kevin VanHorn. All rights reserved.


#include "Public/PUserWidget.h"
#include "Components/ScaleBox.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetMaterialLibrary.h"

void UPUserWidget::NativeConstruct() {
	Super::NativeConstruct();
}

void UPUserWidget::SetTouchDragPosition(FVector2D TouchPos)
{
	this->DebugTouchPosition(TouchPos);
	return;
	if (HandWidget) {
		float Scale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
		FVector2D Size = HandWidget->GetCachedGeometry().GetLocalSize();
		
		HandWidget->SetRenderTranslation((TouchPos / Scale));

		//FGeometry Geometry = HandWidget->GetCachedGeometry();
		//FVector2D Position = Geometry.AbsoluteToLocal(HandWidget->GetCachedGeometry().GetAbsolutePosition()) + HandWidget->GetCachedGeometry().GetLocalSize() / 2.0f;
	}
}

void UPUserWidget::UpdateScore(int32 ScoreIn)
{
	if (ScoreTextWidget) {
		ScoreTextWidget->SetText(FText::AsNumber(ScoreIn));
	}
}

void UPUserWidget::UpdateHealth(float HealthIn)
{
	if (MPC_Score) {
		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC_Score, FName("Health"), HealthIn);
	}
}

void UPUserWidget::SetHealthOver(float HealthOver)
{
	if (MPC_Score) {
		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC_Score, FName("HealthOver"), HealthOver);
	}
}

void UPUserWidget::UpdateUIDifficulty(EDifficultyType DiffType)
{
	this->OnUpdateUIDifficulty(DiffType);
}

void UPUserWidget::SetDTimeRemaining(float TimeRemaining)
{
	TimeToNextDifficulty = TimeRemaining;
	if (DTimerWidget) {
		int32 iTime = FMath::CeilToInt(TimeRemaining);
		if (iTime >= 0) {
			DTimerWidget->SetText(FText::AsNumber(iTime));
		}
		else {
			DTimerWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
