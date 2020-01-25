// Copyright 2019, Kevin VanHorn. All rights reserved.


#include "PScrollBox.h"
#include "ScrollBox.h"

UPScrollBox::UPScrollBox() {

}

void UPScrollBox::PostLoad()
{
	Super::PostLoad();
	UScrollBox::OnUserScrolled.AddUniqueDynamic(this, &UPScrollBox::OnUserScrolled);
	ScrollToStart();
}

void UPScrollBox::OnUserScrolled(float CurrentOffset)
{
	int test = 1;
}
