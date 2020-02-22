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
	ETiger		UMETA(DisplayName = "Tiger"),
};

UENUM(BlueprintType)
enum class EDogChoice : uint8
{
	EDefault 	UMETA(DisplayName = "Default"),
	EPug 	UMETA(DisplayName = "Pug"),
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

	void PInit(class APUmbrella* UmbrellaIn, class UStaticMeshComponent* UIMesh, class USkeletalMeshComponent* UIPuppy);

	void SetPPlayerState(class APPlayerState* PPlayerStateIn) { PPlayerState = PPlayerStateIn; }

	UFUNCTION(BlueprintCallable)
		void SelectUmbrellaPattern(EUmbrellaPattern UmbrellaPattern);

	UFUNCTION(BlueprintCallable)
		void SelectDogChoice(EDogChoice DogChoice);

	UFUNCTION(BlueprintCallable)
		bool BuyUmbrellaPattern(EUmbrellaPattern UmbrellaPattern);

	UFUNCTION(BlueprintCallable)
		bool BuyDog(EDogChoice DogChoice);

	int32 GetCost(EUmbrellaPattern UmbrellaPattern);
	int32 GetCost(EDogChoice DogChoice);

	UFUNCTION(BlueprintCallable)
		TSubclassOf<class APPuppyCharacter> GetDogChoice();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FUmbrellaMaterials> UmbrellaMaterials;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<int32> DogCosts;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<TSubclassOf<class APPuppyCharacter>> DogChoices;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<class USkeletalMesh*> DogSkeletalMeshes;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<class UAnimationAsset*> DogAnimations;

	class APUmbrella* Umbrella;

	class UStaticMeshComponent* UmbrellaMeshUI;

	class USkeletalMeshComponent* PuppyMeshUI;

	class APPlayerState* PPlayerState;

private:
	EDogChoice SelectedDogChoice;
};
