// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <ProceduralMeshComponent.h>
#include "RoofAndFloorActor.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API ARoofAndFloorActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARoofAndFloorActor();

	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* ProceduralMesh;

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* LineMesh;

	void GenerateFloor(const TArray<FVector>& Vertices, int);
	void GenerateRoof(const TArray<FVector>& Vertices, int);
	void GenerateMesh(const TArray<FVector>& Vertices, FString MeshType);

	void ApplyMaterial(UMaterialInstance* Material);

	void GenerateOverviewMesh(const FVector& StartPoint, const FVector& EndPoint, float Width, float Height);

	UPROPERTY()
	TArray<FVector> RoofFloorVertices;

	UPROPERTY()
	FString ActorType;

	UPROPERTY()
	int RoofAtStorey = -1;

	UPROPERTY()
	int FloorAtStorey = -1;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
