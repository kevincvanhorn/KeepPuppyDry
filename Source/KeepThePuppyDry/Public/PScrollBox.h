// Copyright 2019, Kevin VanHorn. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ScrollBox.h"
#include "PScrollBox.generated.h"

/**
 * 
 */
UCLASS()
class KEEPTHEPUPPYDRY_API UPScrollBox : public UScrollBox
{
	GENERATED_BODY()

public:
    UPScrollBox();

    virtual void PostLoad();

protected:
    UFUNCTION()
       void OnUserScrolled(float CurrentOffset);

};
