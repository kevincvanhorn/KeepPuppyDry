// Copyright 2019, Kevin VanHorn. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Particles/Emitter.h"
#include "PRainEmitter.generated.h"

/**
 * 
 */
UCLASS()
class KEEPTHEPUPPYDRY_API APRainEmitter : public AEmitter
{
	GENERATED_BODY()
	
public:
	APRainEmitter();

	UFUNCTION(BlueprintCallable)
		void SetAcceleration(FVector DirectionIn);

protected:
	virtual void BeginPlay() override;

private:
	UParticleSystemComponent* ParticleSystemComp;
};
