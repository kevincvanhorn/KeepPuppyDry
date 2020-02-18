// Copyright 2020, Kevin VanHorn. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SkeletalMeshActor.h"
#include "PPreviewPuppy.generated.h"

/**
 * 
 */
UCLASS()
class KEEPTHEPUPPYDRY_API APPreviewPuppy : public ASkeletalMeshActor
{
	GENERATED_BODY()

public:
	APPreviewPuppy();
	
	UFUNCTION(BlueprintCallable)
		void UpdateAnimationTick(float DeltaTime, bool bNeedsValidRootMotion);

};
