// Copyright 2020, Kevin VanHorn. All rights reserved.


#include "PRainEmitter.h"
#include "Particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"

APRainEmitter::APRainEmitter() {
}

void APRainEmitter::SetAcceleration(FVector DirectionIn, float RainIntensity)
{
	if (ParticleSystemComp) {
		//ParticleSystemComp->SetVectorParameter("PRain_Accel", DirectionIn);
		ParticleSystemComp->SetVectorParameter("PRain_Accel", DirectionIn * RainIntensity);
		ParticleSystemComp->SetVectorParameter("PRain_Velocity", DirectionIn * RainIntensity);
	}
}

void APRainEmitter::BeginPlay()
{
	Super::BeginPlay();
	ParticleSystemComp = GetParticleSystemComponent();
}
