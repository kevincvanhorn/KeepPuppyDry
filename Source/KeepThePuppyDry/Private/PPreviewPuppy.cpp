// Copyright 2020, Kevin VanHorn. All rights reserved.


#include "PPreviewPuppy.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"

APPreviewPuppy::APPreviewPuppy() {
	
}

void APPreviewPuppy::UpdateAnimationTick(float DeltaTime, bool bNeedsValidRootMotion)
{
	USkeletalMeshComponent* SkeletalMeshComp = GetSkeletalMeshComponent();
	if (SkeletalMeshComp) {
		SkeletalMeshComp->TickAnimation(DeltaTime, bNeedsValidRootMotion);
	}
}
