// Copyright 2019, Kevin VanHorn. All rights reserved.


#include "PRainEmitter.h"
#include "Particles/ParticleSystemComponent.h"

APRainEmitter::APRainEmitter() {

}

void APRainEmitter::SetAcceleration(FVector DirectionIn)
{
	if (ParticleSystemComp) {
		ParticleSystemComp->SetVectorParameter("PRain_Accel", DirectionIn);
	}
}

void APRainEmitter::BeginPlay()
{
	Super::BeginPlay();
	ParticleSystemComp = GetParticleSystemComponent();
}
