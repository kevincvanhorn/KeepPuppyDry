// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/PUserWidget.h"
#include "Components/ScaleBox.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UPUserWidget::NativeConstruct() {
	Super::NativeConstruct();
}

void UPUserWidget::SetTouchDragPosition(FVector2D TouchPos)
{
	this->DebugTouchPosition(TouchPos);
	if (HandWidget) {
		float Scale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
		FVector2D Size = HandWidget->GetCachedGeometry().GetLocalSize();
		
		HandWidget->SetRenderTranslation((TouchPos / Scale));

		//FGeometry Geometry = HandWidget->GetCachedGeometry();
		//FVector2D Position = Geometry.AbsoluteToLocal(HandWidget->GetCachedGeometry().GetAbsolutePosition()) + HandWidget->GetCachedGeometry().GetLocalSize() / 2.0f;

		//UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(HandWidget->Slot);
		//Slot->SetPosition(Position);
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *Position.ToString());

	}
}
