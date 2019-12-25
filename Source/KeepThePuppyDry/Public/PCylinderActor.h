// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "PCylinderActor.generated.h"

UCLASS()
class KEEPTHEPUPPYDRY_API APCylinderActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APCylinderActor();

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostActorCreated() override;

	virtual void PostLoad() override;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cylinder Parameters")
		float Radius = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cylinder Parameters")
		float Height = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cylinder Parameters")
		int32 CrossSectionCount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cylinder Parameters")
		bool bCapEnds = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cylinder Parameters")
		bool bDoubleSided = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cylinder Parameters")
		bool bSmoothNormals = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cylinder Parameters")
		FVector TopCapPosition; // This controls where the cylinder will go.

private:
	UPROPERTY(VisibleAnywhere)
		UProceduralMeshComponent* Mesh;

	void CreateMesh();

	void RefreshMesh(); // Update mesh with new offset for top cap/ring of vertices.

private:
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;
	TArray<FColor> VertexColors;

private:
	TArray<int> TopCapIndices;
	TArray<int> BotCapIndices;
	FVector PrevTopPosition;
};
