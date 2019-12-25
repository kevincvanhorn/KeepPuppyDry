// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "PProcedualMeshActor.generated.h"

UCLASS()
class KEEPTHEPUPPYDRY_API APProcedualMeshActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APProcedualMeshActor();

	void SetUParent(class APUmbrella* UmbrellaIn);

	void SetUOffset(FVector OffsetIn) { UOffset = OffsetIn; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
		UProceduralMeshComponent* Mesh;

protected:
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
	void CreateMesh();

	void RefreshMesh(); // Update mesh with new offset for top cap/ring of vertices.

private:
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;
	TArray<FColor> VertexColors;

	TArray<int> TopCapIndices;
	TArray<int> BotCapIndices;
	FVector PrevTopPosition;

private:
	class APUmbrella* Umbrella;
	FVector UOffset;
};
