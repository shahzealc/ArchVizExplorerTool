// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WallActor.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API AWallActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWallActor();

	void GenerateSegmentedWalls(int Segments);
	void DestroyComponentByTransform(const FTransform& Transform);
	void GenerateDoor(FTransform, UStaticMesh*);
	void GenerateWindow(FTransform, UStaticMesh*, UMaterialInterface*);
	void ApplyMaterial(UMaterialInstance*);
	int32 GetNumSegments() const;
	void DestroyPreviousWalls();

	UPROPERTY()
	USceneComponent* RootScene;

	UPROPERTY()
	TArray<UStaticMeshComponent*> LowerWallArray;

	UPROPERTY()
	TArray<UStaticMeshComponent*> UpperWallArray;

	UPROPERTY()
	UStaticMesh* WallMesh;	
	
	UPROPERTY()
	UStaticMesh* FillerMesh;

	UPROPERTY()
	int NumberofSegments;

	UPROPERTY()
	UMaterialInstance* WallMaterial;

	UPROPERTY()
	TArray<FTransform> DoorTransformArray;

	UPROPERTY()
	TArray<UStaticMesh*> DoorStaticMeshArray;
	
	UPROPERTY()
	TArray<FTransform> WindowTransformArray;

	UPROPERTY()
	TArray<UStaticMesh*> WindowStaticMeshArray;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
