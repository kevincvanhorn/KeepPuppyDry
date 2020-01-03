// Copyright 2019, Kevin VanHorn. All rights reserved.


#include "PRainEmitter.h"
#include "Particles/ParticleSystemComponent.h"

APRainEmitter::APRainEmitter() {
}

void APRainEmitter::SetAcceleration(FVector DirectionIn, float RainIntensity)
{
	if (ParticleSystemComp) {
		//ParticleSystemComp->SetVectorParameter("PRain_Accel", DirectionIn);
		ParticleSystemComp->SetVectorParameter("PRain_Accel", DirectionIn * RainIntensity);
		//ParticleSystemComp->SetVectorParameter("PRain_Velocity", DirectionIn * RainIntensity * 0.5f);
		//UE_LOG(LogTemp, Warning, TEXT("Direction: %s"), *(DirectionIn.ToString()));
	}
}

void APRainEmitter::BeginPlay()
{
	Super::BeginPlay();
	ParticleSystemComp = GetParticleSystemComponent();
}
