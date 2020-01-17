// Copyright 2019, Kevin VanHorn. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PGameInstance.generated.h"

/**
 * Persistent through play session.
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
		TSubclassOf<class UUserWidget> WidgetLoadingScreenClass;

	UFUNCTION()
		virtual void BeginLoadingScreen(const FString& MapName);
	UFUNCTION()
		virtual void EndLoadingScreen(UWorld* InLoadedWorld);

	bool ShouldSkipToGameplay() const { return bSkipToGameplay; }

	void SetSkipToGameplay(bool bSkip) { bSkipToGameplay = bSkip; }

private:
	class UUserWidget* LoadingScreenWidget;
	
	bool bSkipToGameplay;

public:
	int32 IncrementNumSessionLosses() { return ++NumSessionLosses; }

	int32 GetNumSessionLosses() const { return NumSessionLosses; }

private:
	int32 NumSessionLosses;
};
