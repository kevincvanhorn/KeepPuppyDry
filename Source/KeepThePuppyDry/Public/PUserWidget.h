// Copyright 2019, Kevin VanHorn. All rights reserved.

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

	void UpdateScore(int32 ScoreIn);

	void UpdateHealth(float HealthIn);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UScaleBox* HandWidget;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* ScoreTextWidget;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UMaterialParameterCollection* MPC_Score;
};
