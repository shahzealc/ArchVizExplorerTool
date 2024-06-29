// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "RoadActor.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API ARoadActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoadActor();

	void Initialize(FVector Dimention);

	void CreateRoad(float Length, float Width, float Height, TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FVector2D>& UVs, TArray<FProcMeshTangent>& Tangents);

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* RoadProceduralMesh;

	UPROPERTY()
	FVector RoadDimention;

	void ApplyRoadMaterial(UMaterialInstance* Material);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
