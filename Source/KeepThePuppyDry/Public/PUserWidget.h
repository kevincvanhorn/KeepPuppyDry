// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class KEEPTHEPUPPYDRY_API UPUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetTouchDragPosition(FVector2D TouchPos);

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "DebugTouchPosition"))
		void DebugTouchPosition(FVector2D TouchPos);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UScaleBox* HandWidget;
};
