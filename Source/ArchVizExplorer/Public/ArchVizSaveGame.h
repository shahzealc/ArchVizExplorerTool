// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ArchVizSaveGame.generated.h"

USTRUCT()
struct FRoadActorData
{
	GENERATED_BODY()

	UPROPERTY()
	FTransform RoadTransform;

	UPROPERTY()
	UMaterialInstance* RoadMaterial;

	UPROPERTY()
	FVector RoadDimentions;

};

USTRUCT()
struct FWallActorData
{
	GENERATED_BODY()

	UPROPERTY()
	FTransform WallTransform;

	UPROPERTY()
	UMaterialInstance* WallMaterial;

	UPROPERTY()
	int NumberofSegments;

	UPROPERTY()
	TArray<FTransform> DoorTransformArray;

	UPROPERTY()
	TArray<UStaticMesh*> DoorStaticMeshArray;

	UPROPERTY()
	TArray<FTransform> WindowTransformArray;

	UPROPERTY()
	TArray<UStaticMesh*> WindowStaticMeshArray;

};

USTRUCT()
struct FRoofFloorActorData
{
	GENERATED_BODY()

	UPROPERTY()
	FTransform RoofFloorTransform;

	UPROPERTY()
	UMaterialInstance* RoofFloorMaterial;

	UPROPERTY()
	TArray<FVector> RoofFloorVertices;

	UPROPERTY()
	FString ActorType;

	UPROPERTY()
	int RoofAtStorey;
};

USTRUCT()
struct FInteriorActorData
{
	GENERATED_BODY()

	UPROPERTY()
	FTransform InteriorTransform;

	UPROPERTY()
	UStaticMesh* InteriorMesh;

	UPROPERTY()
	FString InteriorActorType;

};

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API UArchVizSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TArray<FRoadActorData> RoadActorArray;

	UPROPERTY()
	TArray<FWallActorData> WallActorArray;

	UPROPERTY()
	TArray<FRoofFloorActorData> RoofFloorActorArray;

	UPROPERTY()
	TArray<FInteriorActorData> InteriorActorArray;

};
