// Copyright 2019, Kevin VanHorn. All rights reserved.


#include "PCustomizationManager.h"
#include "Components/StaticMeshComponent.h"
#include "PUmbrella.h"
#include "PPlayerState.h"
#include "PPuppyCharacter.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
APCustomizationManager::APCustomizationManager()
{
	SelectedDogChoice = EDogChoice::EDefault;
}

void APCustomizationManager::PInit(APUmbrella* UmbrellaIn, UStaticMeshComponent* UIMesh, class USkeletalMeshComponent* UIPuppy)
{
	Umbrella = UmbrellaIn;
	UmbrellaMeshUI = UIMesh;
	PuppyMeshUI = UIPuppy;
}

void APCustomizationManager::SelectUmbrellaPattern(EUmbrellaPattern UmbrellaPattern)
{
	if ((uint8)UmbrellaPattern < UmbrellaMaterials.Num()) {
		if (Umbrella && PPlayerState) {
			if (PPlayerState->PlayerOwnsAsset(UmbrellaPattern)) {
				PPlayerState->SetLastSelected(UmbrellaPattern);
				PPlayerState->SaveGame();
				Umbrella->SetMaterial(0, UmbrellaMaterials[(uint8)UmbrellaPattern].UnderMaterial);
				Umbrella->SetMaterial(1, UmbrellaMaterials[(uint8)UmbrellaPattern].TopMaterial);
				Umbrella->SetMaterial(2, UmbrellaMaterials[(uint8)UmbrellaPattern].UnderMaterial);
				Umbrella->SetMaterial(3, UmbrellaMaterials[(uint8)UmbrellaPattern].HandleMaterial);
			}
		}
		if (UmbrellaMeshUI) {
			UmbrellaMeshUI->SetMaterial(0, UmbrellaMaterials[(uint8)UmbrellaPattern].UnderMaterial);
			UmbrellaMeshUI->SetMaterial(1, UmbrellaMaterials[(uint8)UmbrellaPattern].TopMaterialUI);
			UmbrellaMeshUI->SetMaterial(2, UmbrellaMaterials[(uint8)UmbrellaPattern].UnderMaterial);
			UmbrellaMeshUI->SetMaterial(3, UmbrellaMaterials[(uint8)UmbrellaPattern].HandleMaterial);
		}
	}
}

void APCustomizationManager::SelectDogChoice(EDogChoice DogChoice)
{
	if ((uint8)DogChoice < DogChoices.Num()) {
		if (PPlayerState) {
			if (PPlayerState->PlayerOwnsAsset(DogChoice)) {
				SelectedDogChoice = DogChoice;
				PPlayerState->SetLastSelected(DogChoice);
				PPlayerState->SaveGame();
			}
		}
		if (PuppyMeshUI) {
			if ((uint8)DogChoice < DogSkeletalMeshes.Num()) {
				//USkeletalMesh* Mesh = DogChoices[(uint8)SelectedDogChoice].GetDefaultObject()->GetMesh()->SkeletalMesh;
				USkeletalMesh* Mesh = DogSkeletalMeshes[(uint8)DogChoice];
				PuppyMeshUI->SetSkeletalMesh(Mesh);
			}
		}
	}
}

bool APCustomizationManager::BuyUmbrellaPattern(EUmbrellaPattern UmbrellaPattern)
{
	if (PPlayerState) {
		if ((uint8)UmbrellaPattern < UmbrellaMaterials.Num()) {
			int32 Cost = UmbrellaMaterials[(uint8)UmbrellaPattern].Cost;
			return PPlayerState->BuyAsset(UmbrellaPattern, Cost);
		}
	}
	return false;
}

bool APCustomizationManager::BuyDog(EDogChoice DogChoice)
{
	if (PPlayerState) {
		if ((uint8)DogChoice < DogCosts.Num()) {
			int32 Cost = DogCosts[(uint8)DogChoice];
			return PPlayerState->BuyAsset(DogChoice, Cost);
		}
	}
	return false;
}

int32 APCustomizationManager::GetCost(EUmbrellaPattern UmbrellaPattern)
{
	if ((uint8)UmbrellaPattern < UmbrellaMaterials.Num()) {
		return UmbrellaMaterials[(uint8)UmbrellaPattern].Cost;
	}
	return 0;
}

int32 APCustomizationManager::GetCost(EDogChoice DogChoice)
{
	if ((uint8)DogChoice < DogCosts.Num()) {
		return DogCosts[(uint8)DogChoice];
	}
	return 0;
}

TSubclassOf<class APPuppyCharacter> APCustomizationManager::GetDogChoice()
{
	if ((int32)SelectedDogChoice < DogChoices.Num() && DogChoices.Num() > 0) {
		return DogChoices[(int32)SelectedDogChoice];
	}
	return nullptr;
}

// Called when the game starts or when spawned
void APCustomizationManager::BeginPlay()
{
	Super::BeginPlay();
	
}

