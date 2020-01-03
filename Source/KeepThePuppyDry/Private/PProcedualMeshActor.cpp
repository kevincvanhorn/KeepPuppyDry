// Copyright 2020, Kevin VanHorn. All rights reserved.


#include "PProcedualMeshActor.h"
#include "PUmbrella.h"
#include "DrawDebugHelpers.h"

// Sets default values
APProcedualMeshActor::APProcedualMeshActor()
{
	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	if (Mesh) {
		Mesh->bUseAsyncCooking = true; // New in UE 4.17, multi-threaded PhysX cooking.
		SetRootComponent(Mesh);
	}

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APProcedualMeshActor::SetUParent(APUmbrella* UmbrellaIn)
{
	Umbrella = UmbrellaIn;
}

void APProcedualMeshActor::SetCapLocationXY(const FVector2D& Loc)
{
	TopCapPosition.X = Loc.X;
	TopCapPosition.Y = Loc.Y;
}

void APProcedualMeshActor::SetCapLocationXY(const FVector2D& Loc, float CylinderOffset)
{
	TopCapPosition.X = Loc.X;
	TopCapPosition.Y = Loc.Y;
	//FVector A = GetActorLocation();
	//FVector B = A + TopCapPosition.GetSafeNormal() * FVector(1, -1, 1)*1000;
}

void APProcedualMeshActor::ProjectCapLocation(FVector& ProjectedLoc, float FloorHeight)
{
	FVector A = GetActorLocation();
	FVector B = A + TopCapPosition*FVector(1,-1,1);
	ProjectedLoc = A + ((FloorHeight-A.Z)/(B.Z - A.Z))*(B-A);
}

// Called when the game starts or when spawned
void APProcedualMeshActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APProcedualMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Umbrella) {
		SetActorLocation(Umbrella->GetActorLocation() + UOffset);
	}

	if (PrevTopPosition != TopCapPosition) {
		RefreshMesh();
		PrevTopPosition = TopCapPosition;
	}
}

//@source https://wiki.unrealengine.com/Procedural_Mesh_Component_in_C%2B%2B:Getting_Started
// This is called when actor is spawned (at runtime or when you drop it into the world in editor)
void APProcedualMeshActor::PostActorCreated()
{
	Super::PostActorCreated();
	CreateMesh();
}

// This is called when actor is already in level and map is opened
void APProcedualMeshActor::PostLoad()
{
	Super::PostLoad();
	CreateMesh();
}

// @source: https://forums.unrealengine.com/development-discussion/c-gameplay-programming/1674-generate-procedural-mesh/page24
void APProcedualMeshActor::CreateMesh()
{
	int VertexIndex = 0;
	int32 NumVerts = CrossSectionCount * 4;

	// Count extra vertices if double sided
	if (bDoubleSided)
	{
		NumVerts = NumVerts * 2;
	}
	// Count vertices for caps if set
	if (bCapEnds)
	{
		NumVerts += 2 * (CrossSectionCount - 1) * 3;
	}

	// Initialize Arrays:
	Triangles.Reset();

	Vertices.Reset();
	Vertices.AddUninitialized(NumVerts);

	Normals.Reset();
	Normals.AddUninitialized(NumVerts);

	Tangents.Reset();
	Tangents.AddUninitialized(NumVerts);

	UVs.Reset();
	UVs.AddUninitialized(NumVerts);

	// -------------------------------------------------------
	// Make a cylinder section
	const float AngleBetweenQuads = (2.0f / (float)(CrossSectionCount)) * PI;
	const float VMapPerQuad = 1.0f / (float)CrossSectionCount;
	//FVector Offset = FVector(0, 0, Height);
	FVector Offset = TopCapPosition;

	// Start by building up vertices that make up the cylinder sides
	for (int32 QuadIndex = 0; QuadIndex < CrossSectionCount; QuadIndex++)
	{
		float Angle = (float)QuadIndex * AngleBetweenQuads;
		float NextAngle = (float)(QuadIndex + 1) * AngleBetweenQuads;

		// Set up the vertices
		FVector p0 = FVector(FMath::Cos(Angle) * Radius, FMath::Sin(Angle) * Radius, 0.f);
		FVector p1 = FVector(FMath::Cos(NextAngle) * Radius, FMath::Sin(NextAngle) * Radius, 0.f);
		FVector p2 = p1 + Offset;
		FVector p3 = p0 + Offset;

		// Set up the quad triangles
		int VertIndex1 = VertexIndex++;
		int VertIndex2 = VertexIndex++;
		int VertIndex3 = VertexIndex++;
		int VertIndex4 = VertexIndex++;

		Vertices[VertIndex1] = p0; BotCapIndices.Add(VertIndex2);
		Vertices[VertIndex2] = p1; BotCapIndices.Add(VertIndex1);
		Vertices[VertIndex3] = p2; TopCapIndices.Add(VertIndex3);
		Vertices[VertIndex4] = p3; TopCapIndices.Add(VertIndex4);

		// Now create two triangles from those four vertices
		// The order of these (clockwise/counter-clockwise) dictates which way the normal will face. 
		Triangles.Add(VertIndex4);
		Triangles.Add(VertIndex3);
		Triangles.Add(VertIndex1);

		Triangles.Add(VertIndex3);
		Triangles.Add(VertIndex2);
		Triangles.Add(VertIndex1);

		// UVs
		UVs[VertIndex1] = FVector2D(VMapPerQuad * QuadIndex, 0.0f);
		UVs[VertIndex2] = FVector2D(VMapPerQuad * (QuadIndex + 1), 0.0f);
		UVs[VertIndex3] = FVector2D(VMapPerQuad * (QuadIndex + 1), 1.0f);
		UVs[VertIndex4] = FVector2D(VMapPerQuad * QuadIndex, 1.0f);

		// Normals
		FVector NormalCurrent = FVector::CrossProduct(Vertices[VertIndex1] - Vertices[VertIndex3], Vertices[VertIndex2] - Vertices[VertIndex3]).GetSafeNormal();

		if (bSmoothNormals)
		{
			// To smooth normals you give the vertices a different normal value than the polygon they belong to, gfx hardware then knows how to interpolate between those.
			// I do this here as an average between normals of two adjacent polygons
			float NextNextAngle = (float)(QuadIndex + 2) * AngleBetweenQuads;
			FVector p4 = FVector(FMath::Cos(NextNextAngle) * Radius, FMath::Sin(NextNextAngle) * Radius, 0.f);

			// p1 to p4 to p2
			FVector NormalNext = FVector::CrossProduct(p1 - p2, p4 - p2).GetSafeNormal();
			FVector AverageNormalRight = (NormalCurrent + NormalNext) / 2;
			AverageNormalRight = AverageNormalRight.GetSafeNormal();

			float PreviousAngle = (float)(QuadIndex - 1) * AngleBetweenQuads;
			FVector pMinus1 = FVector(FMath::Cos(PreviousAngle) * Radius, FMath::Sin(PreviousAngle) * Radius, 0.f);

			// p0 to p3 to pMinus1
			FVector NormalPrevious = FVector::CrossProduct(p0 - pMinus1, p3 - pMinus1).GetSafeNormal();
			FVector AverageNormalLeft = (NormalCurrent + NormalPrevious) / 2;
			AverageNormalLeft = AverageNormalLeft.GetSafeNormal();

			Normals[VertIndex1] = AverageNormalLeft;
			Normals[VertIndex2] = AverageNormalRight;
			Normals[VertIndex3] = AverageNormalRight;
			Normals[VertIndex4] = AverageNormalLeft;
		}
		else
		{
			// If not smoothing we just set the vertex normal to the same normal as the polygon they belong to
			Normals[VertIndex1] = NormalCurrent;
			Normals[VertIndex2] = NormalCurrent;
			Normals[VertIndex3] = NormalCurrent;
			Normals[VertIndex4] = NormalCurrent;
		}

		// Tangents (perpendicular to the surface)
		FVector SurfaceTangent = p0 - p1;
		SurfaceTangent = SurfaceTangent.GetSafeNormal();
		Tangents[VertIndex1] = FProcMeshTangent(SurfaceTangent, true);
		Tangents[VertIndex2] = FProcMeshTangent(SurfaceTangent, true);
		Tangents[VertIndex3] = FProcMeshTangent(SurfaceTangent, true);
		Tangents[VertIndex4] = FProcMeshTangent(SurfaceTangent, true);

		// If double sides, create extra polygons but face the normals the other way.
		if (bDoubleSided)
		{
			VertIndex1 = VertexIndex++;
			VertIndex2 = VertexIndex++;
			VertIndex3 = VertexIndex++;
			VertIndex4 = VertexIndex++;

			Vertices[VertIndex1] = p0;
			Vertices[VertIndex2] = p1;
			Vertices[VertIndex3] = p2;
			Vertices[VertIndex4] = p3;

			Triangles.Add(VertIndex1);
			Triangles.Add(VertIndex3);
			Triangles.Add(VertIndex4);

			Triangles.Add(VertIndex2);
			Triangles.Add(VertIndex3);
			Triangles.Add(VertIndex4);
		}

		if (QuadIndex != 0 && bCapEnds)
		{
			// Cap is closed by triangles that start at 0, then use the points at the angles for the other corners

			// Bottom
			FVector capVertex0 = FVector(FMath::Cos(0) * Radius, FMath::Sin(0) * Radius, 0.f);
			FVector capVertex1 = FVector(FMath::Cos(Angle) * Radius, FMath::Sin(Angle) * Radius, 0.f);
			FVector capVertex2 = FVector(FMath::Cos(NextAngle) * Radius, FMath::Sin(NextAngle) * Radius, 0.f);

			VertIndex1 = VertexIndex++;
			VertIndex2 = VertexIndex++;
			VertIndex3 = VertexIndex++;
			Vertices[VertIndex1] = capVertex0;
			Vertices[VertIndex2] = capVertex1;
			Vertices[VertIndex3] = capVertex2;

			Triangles.Add(VertIndex1);
			Triangles.Add(VertIndex2);
			Triangles.Add(VertIndex3);

			FVector2D UV1 = FVector2D(FMath::Sin(0), FMath::Cos(0));
			FVector2D UV2 = FVector2D(FMath::Sin(Angle), FMath::Cos(Angle));
			FVector2D UV3 = FVector2D(FMath::Sin(NextAngle), FMath::Cos(NextAngle));

			UVs[VertIndex1] = UV1;
			UVs[VertIndex2] = UV2;
			UVs[VertIndex3] = UV3;

			// Top
			capVertex0 = capVertex0 + Offset;
			capVertex1 = capVertex1 + Offset;
			capVertex2 = capVertex2 + Offset;

			VertIndex1 = VertexIndex++;
			VertIndex2 = VertexIndex++;
			VertIndex3 = VertexIndex++;
			Vertices[VertIndex1] = capVertex0;
			Vertices[VertIndex2] = capVertex1;
			Vertices[VertIndex3] = capVertex2;

			Triangles.Add(VertIndex3);
			Triangles.Add(VertIndex2);
			Triangles.Add(VertIndex1);

			UVs[VertIndex1] = UV1;
			UVs[VertIndex2] = UV2;
			UVs[VertIndex3] = UV3;
		}
	}

	Mesh->ClearAllMeshSections();
	Mesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, false);
	Mesh->ContainsPhysicsTriMeshData(false); // Collision data
}

void APProcedualMeshActor::RefreshMesh()
{
	FVector Offset = TopCapPosition;
	for (int i = 0; i < TopCapIndices.Num() && i < BotCapIndices.Num(); ++i) {
		Vertices[TopCapIndices[i]] = Vertices[BotCapIndices[i]] + Offset;
	}
	/*
	FVector p2 = p1 + Offset;
	FVector p3 = p0 + Offset;
	*/

	Mesh->ClearAllMeshSections();
	Mesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, false);
	Mesh->ContainsPhysicsTriMeshData(false); // Collision data
}