// Copyright 2019, Kevin VanHorn. All rights reserved.


#include "PCustomizationManager.h"
#include "Components/StaticMeshComponent.h"
#include "PUmbrella.h"

// Sets default values
APCustomizationManager::APCustomizationManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APCustomizationManager::PInit(APUmbrella* UmbrellaIn, UStaticMeshComponent* UIMesh)
{
	Umbrella = UmbrellaIn;
	UmbrellaMeshUI = UIMesh;
}

void APCustomizationManager::SelectUmbrellaPattern(EUmbrellaPattern UmbrellaPattern)
{
	UE_LOG(LogTemp, Warning, TEXT("SET UMBRELLA PATTERN 3"));
	if ((uint8)UmbrellaPattern < UmbrellaMaterials.Num()) {
		if (Umbrella) {
			Umbrella->SetMaterial(0, UmbrellaMaterials[(uint8)UmbrellaPattern].UnderMaterial);
			Umbrella->SetMaterial(1, UmbrellaMaterials[(uint8)UmbrellaPattern].TopMaterial);
			Umbrella->SetMaterial(2, UmbrellaMaterials[(uint8)UmbrellaPattern].UnderMaterial);
			Umbrella->SetMaterial(3, UmbrellaMaterials[(uint8)UmbrellaPattern].HandleMaterial);
		}
		if (UmbrellaMeshUI) {
			UmbrellaMeshUI->SetMaterial(0, UmbrellaMaterials[(uint8)UmbrellaPattern].UnderMaterial);
			UmbrellaMeshUI->SetMaterial(1, UmbrellaMaterials[(uint8)UmbrellaPattern].TopMaterialUI);
			UmbrellaMeshUI->SetMaterial(2, UmbrellaMaterials[(uint8)UmbrellaPattern].UnderMaterial);
			UmbrellaMeshUI->SetMaterial(3, UmbrellaMaterials[(uint8)UmbrellaPattern].HandleMaterial);
		}
	}
}

// Called when the game starts or when spawned
void APCustomizationManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APCustomizationManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

