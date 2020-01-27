// Copyright 2019, Kevin VanHorn. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PCustomizationManager.generated.h"

UENUM(BlueprintType)
enum class EUmbrellaPattern : uint8
{
	EHearts 	UMETA(DisplayName = "Hearts"),
	EGiraffe 	UMETA(DisplayName = "Giraffe"),
	EZebra		UMETA(DisplayName = "Zebra"),
	ETiger		UMETA(DisplayName = "Tiger")
};

USTRUCT(BlueprintType)
struct FUmbrellaMaterials
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UMaterialInterface* TopMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UMaterialInterface* TopMaterialUI;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UMaterialInterface* UnderMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UMaterialInterface* HandleMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Cost;
};

UCLASS()
class KEEPTHEPUPPYDRY_API APCustomizationManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APCustomizationManager();

	void PInit(class APUmbrella* UmbrellaIn, class UStaticMeshComponent* UIMesh);

	void SetPPlayerState(class APPlayerState* PPlayerStateIn) { PPlayerState = PPlayerStateIn; }

	UFUNCTION(BlueprintCallable)
		void SelectUmbrellaPattern(EUmbrellaPattern UmbrellaPattern);

	UFUNCTION(BlueprintCallable)
		bool BuyUmbrellaPattern(EUmbrellaPattern UmbrellaPattern);

	int32 GetCost(EUmbrellaPattern UmbrellaPattern);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FUmbrellaMaterials> UmbrellaMaterials;

	class APUmbrella* Umbrella;

	class UStaticMeshComponent* UmbrellaMeshUI;

	class APPlayerState* PPlayerState;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
