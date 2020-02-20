// Copyright 2020, Kevin VanHorn. All rights reserved.


#include "PPreviewPuppy.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"

APPreviewPuppy::APPreviewPuppy() {
	
}

void APPreviewPuppy::UpdateAnimationTick(float DeltaTime, bool bNeedsValidRootMotion)
{
	USkeletalMeshComponent* SkeletalMeshComp = GetSkeletalMeshComponent();
	if (SkeletalMeshComp) {
		SkeletalMeshComp->TickAnimation(DeltaTime, bNeedsValidRootMotion);
		SkeletalMeshComp->AnimScriptInstance->bNeedsUpdate = true;
		SkeletalMeshComp->AnimScriptInstance->UpdateAnimation(DeltaTime, false, UAnimInstance::EUpdateAnimationFlag::ForceParallelUpdate);
		SkeletalMeshComp->AnimScriptInstance->NativeUpdateAnimation(DeltaTime);
	}
}
