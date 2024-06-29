// Fill out your copyright notice in the Description page of Project Settings.


#include "RoofAndFloorActor.h"

// Sets default values
ARoofAndFloorActor::ARoofAndFloorActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;
}

void ARoofAndFloorActor::GenerateFloor(const TArray<FVector>& Vertices, int FloorAtStorey_)
{
	FloorAtStorey = FloorAtStorey_;
	GenerateMesh(Vertices, "Floor");
}

void ARoofAndFloorActor::GenerateRoof(const TArray<FVector>& Vertices,int RoofAtStorey_)
{
	RoofAtStorey = RoofAtStorey_;
	GenerateMesh(Vertices, "Roof");
}

void ARoofAndFloorActor::GenerateMesh(const TArray<FVector>& Vertices_, FString MeshType)
{
	if (Vertices_.Num() != 4) {
		return;
	}

	ActorType = MeshType;

	float Height;
	if (MeshType == "Floor") {
		Height = 1;
	}
	else if (MeshType == "Roof") {
		Height = 5;
	}

	ProceduralMesh = NewObject<UProceduralMeshComponent>(this, *MeshType);
	ProceduralMesh->AttachToComponent(RootScene, FAttachmentTransformRules::KeepRelativeTransform);
	ProceduralMesh->RegisterComponentWithWorld(GetWorld());

	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;

	for (int i = 0; i < 4; ++i) {
		Vertices.Add(Vertices_[i]);

		RoofFloorVertices.Add(Vertices_[i]);
	}

	// Top  vertices
	for (int32 i = 0; i < 4; ++i)
	{
		Vertices.Add(Vertices_[i] + FVector(0, 0, Height));
	}

	// Triangles  bottom 
	Triangles.Append({ 0, 1, 2, 2, 3, 0 });
	// Triangles top 
	Triangles.Append({ 4, 6, 5, 4, 7, 6 });
	// Triangles sides
	Triangles.Append({ 0, 4, 1, 1, 4, 5 });
	Triangles.Append({ 1, 5, 2, 2, 5, 6 });
	Triangles.Append({ 2, 6, 3, 3, 6, 7 });
	Triangles.Append({ 3, 7, 0, 0, 7, 4 });

	// Normals
	FVector BottomNormal = FVector(0, 0, -1);
	FVector TopNormal = FVector(0, 0, 1);
	FVector SideNormals[4];
	SideNormals[0] = FVector::CrossProduct(Vertices[4] - Vertices[0], FVector(0, 0, 1)).GetSafeNormal();
	SideNormals[1] = FVector::CrossProduct(Vertices[5] - Vertices[1], FVector(0, 0, 1)).GetSafeNormal();
	SideNormals[2] = FVector::CrossProduct(Vertices[6] - Vertices[2], FVector(0, 0, 1)).GetSafeNormal();
	SideNormals[3] = FVector::CrossProduct(Vertices[7] - Vertices[3], FVector(0, 0, 1)).GetSafeNormal();

	// Normals bottom 
	Normals.Append({ BottomNormal, BottomNormal, BottomNormal, BottomNormal });
	// Normals top 
	Normals.Append({ TopNormal, TopNormal, TopNormal, TopNormal });
	//// Normals  sides

	// UVs bottom 
	// UVs top 
	UVs.Append({ FVector2D(1, 1), FVector2D(0, 1), FVector2D(0, 0), FVector2D(1 ,0) });
	UVs.Append({ FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1) });


	// Tangents
	for (int32 i = 0; i < 24; ++i)
	{
		Tangents.Add(FProcMeshTangent(1, 0, 0));
	}

	ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), Tangents, true);

	// Enable collision data
	ProceduralMesh->ContainsPhysicsTriMeshData(true);
}

void ARoofAndFloorActor::ApplyMaterial(UMaterialInstance* Material)
{
	if (IsValid(ProceduralMesh))
		ProceduralMesh->SetMaterial(0, Material);
}

void ARoofAndFloorActor::GenerateOverviewMesh(const FVector& StartPoint, const FVector& EndPoint, float Width, float Height)
{
	LineMesh = NewObject<UProceduralMeshComponent>(this, "Overview");
	LineMesh-> AttachToComponent(RootScene, FAttachmentTransformRules::KeepRelativeTransform);
	LineMesh-> RegisterComponentWithWorld(GetWorld());
	FVector Direction = (EndPoint - StartPoint).GetSafeNormal();
	FVector Right = FVector::CrossProduct(Direction, FVector::UpVector).GetSafeNormal() * Width * 0.5f;
	FVector Up = FVector::UpVector * Height * 0.5f;

	TArray<FVector> Vertices;

	// Bottom face vertices
	Vertices.Add(StartPoint - Right - Up); 
	Vertices.Add(EndPoint - Right - Up);   
	Vertices.Add(EndPoint + Right - Up);   
	Vertices.Add(StartPoint + Right - Up); 

	// Top face vertices
	Vertices.Add(StartPoint - Right + Up); 
	Vertices.Add(EndPoint - Right + Up);   
	Vertices.Add(EndPoint + Right + Up);   
	Vertices.Add(StartPoint + Right + Up); 

	TArray<int32> Triangles;

	// Bottom face
	Triangles.Add(0); Triangles.Add(1); Triangles.Add(2);
	Triangles.Add(2); Triangles.Add(3); Triangles.Add(0);

	// Top face
	Triangles.Add(4); Triangles.Add(7); Triangles.Add(6);
	Triangles.Add(6); Triangles.Add(5); Triangles.Add(4);

	// Side faces
	Triangles.Add(0); Triangles.Add(4); Triangles.Add(5);
	Triangles.Add(5); Triangles.Add(1); Triangles.Add(0);

	Triangles.Add(1); Triangles.Add(5); Triangles.Add(6);
	Triangles.Add(6); Triangles.Add(2); Triangles.Add(1);

	Triangles.Add(2); Triangles.Add(6); Triangles.Add(7);
	Triangles.Add(7); Triangles.Add(3); Triangles.Add(2);

	Triangles.Add(3); Triangles.Add(7); Triangles.Add(4);
	Triangles.Add(4); Triangles.Add(0); Triangles.Add(3);

	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FLinearColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	// Calculate normals and UVs for each face of the line mesh
	for (int32 i = 0; i < Vertices.Num(); i++)
	{
		Normals.Add(FVector(0, 0, 1));
		UVs.Add(FVector2D(0, 0));
		VertexColors.Add(FLinearColor::White);
		Tangents.Add(FProcMeshTangent(1, 0, 0));
	}

	LineMesh-> CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);

}

// Called when the game starts or when spawned
void ARoofAndFloorActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ARoofAndFloorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




