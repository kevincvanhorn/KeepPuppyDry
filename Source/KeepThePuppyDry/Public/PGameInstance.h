// Copyright 2019, Kevin VanHorn. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class KEEPTHEPUPPYDRY_API UPGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPGameInstance();

	virtual void Init() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		float MinimumLoadingScreenDisplayTime;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UUserWidget* WidgetLoadingScreen;

	UFUNCTION()
		virtual void BeginLoadingScreen(const FString& MapName);
	UFUNCTION()
		virtual void EndLoadingScreen(UWorld* InLoadedWorld);
	
};
