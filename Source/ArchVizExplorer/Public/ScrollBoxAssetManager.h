// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ScrollBoxAssetManager.generated.h"


//Road Material
USTRUCT(BlueprintType)
struct FRoadMaterial {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Road Material")
	FString RoadName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Road Material")
	UTexture2D* RoadImage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Road Material")
	UMaterialInstance* RoadMaterial;
};


//Building Material
USTRUCT(BlueprintType)
struct FBuildingMaterial {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Building Material")
	FString BuildingMaterialName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Building Material")
	UTexture2D* BuildingMaterialImage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Building Material")
	UMaterialInstance* BuildingMaterial;
};

//Interior Material
USTRUCT(BlueprintType)
struct FInteriorMaterial {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interior Material")
	FString InteriorMaterialName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interior Material")
	UTexture2D* InteriorMaterialImage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interior Material")
	UMaterialInstance* InteriorMaterial;
};

//Interior Roof
USTRUCT(BlueprintType)
struct FInteriorRoofData {
	GENERATED_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Interior Roof")
	FString InteriorRoofName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Interior Roof")
	UStaticMesh* InteriorRoofMesh;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Interior Roof")
	UTexture2D* InteriorRoofImage;

};

//Interior Floor
USTRUCT(BlueprintType)
struct FInteriorFloorData {
	GENERATED_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Interior Floor")
	FString InteriorFloorName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Interior Floor")
	UStaticMesh* InteriorFloorMesh;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Interior Floor")
	UTexture2D* InteriorFloorImage;

};

//Interior Wall
USTRUCT(BlueprintType)
struct FInteriorWallData {
	GENERATED_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Interior Wall")
	FString InteriorWallName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Interior Wall")
	UStaticMesh* InteriorWallMesh;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Interior Wall")
	UTexture2D* InteriorWallImage;

};

// Door
USTRUCT(BlueprintType)
struct FDoorData {
	GENERATED_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = " Door")
	FString DoorName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = " Door")
	UStaticMesh* DoorMesh;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = " Door")
	UTexture2D* DoorImage;

};

// Window
USTRUCT(BlueprintType)
struct FWindowData {
	GENERATED_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = " Window")
	FString WindowName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = " Window")
	UStaticMesh* WindowMesh;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = " Window")
	UTexture2D* WindowImage;

};

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API UScrollBoxAssetManager : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Road Material Array")
	TArray<FRoadMaterial> RoadMaterial;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Building Material Array")
	TArray<FBuildingMaterial> BuildingMaterial;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Interior Material Array")
	TArray<FInteriorMaterial> InteriorMaterial;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "InteriorRoof Array")
	TArray<FInteriorRoofData> InteriorRoofData;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "InteriorFloor Array")
	TArray<FInteriorFloorData> InteriorFloorData;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "InteriorWall Array")
	TArray<FInteriorWallData> InteriorWallData;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Door Array")
	TArray<FDoorData> DoorData;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Window Array")
	TArray<FWindowData> WindowData;


};
