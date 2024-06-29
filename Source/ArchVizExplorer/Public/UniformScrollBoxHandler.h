// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "ScrollBoxAssetManager.h"
#include "SUniformScrollBox.h"
#include "UniformScrollBoxHandler.generated.h"

// Declare delegates for each asset type
DECLARE_DELEGATE_OneParam(FOnClickRoadMaterial, const FRoadMaterial&)
DECLARE_DELEGATE_OneParam(FOnClickBuildingMaterial, const FBuildingMaterial&)
DECLARE_DELEGATE_OneParam(FOnClickInteriorMaterial, const FInteriorMaterial&)
DECLARE_DELEGATE_OneParam(FOnClickInteriorRoof, const FInteriorRoofData&)
DECLARE_DELEGATE_OneParam(FOnClickInteriorFloor, const FInteriorFloorData&)
DECLARE_DELEGATE_OneParam(FOnClickInteriorWall, const FInteriorWallData&)
DECLARE_DELEGATE_OneParam(FOnClickDoor, const FDoorData&)
DECLARE_DELEGATE_OneParam(FOnClickWindow, const FWindowData&)

UCLASS()
class ARCHVIZEXPLORER_API UUniformScrollBoxHandler : public UWidget
{
	GENERATED_BODY()

public:
	TSharedPtr<SUniformScrollBox> UniformScrollBoxSlate;

	// Delegates for each asset type
	FOnClickRoadMaterial OnClickRoadMaterial;
	FOnClickBuildingMaterial OnClickBuildingMaterial;
	FOnClickInteriorMaterial OnClickInteriorMaterial;
	FOnClickInteriorRoof OnClickInteriorRoof;
	FOnClickInteriorFloor OnClickInteriorFloor;
	FOnClickInteriorWall OnClickInteriorWall;
	FOnClickDoor OnClickDoor;
	FOnClickWindow OnClickWindow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ScrollBox Asset")
	UScrollBoxAssetManager* ScrollBoxAssetManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlateType")
	TEnumAsByte<EAssetTypes> AssetType;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual const FText GetPaletteCategory() override;
	void ReleaseSlateResources(bool ReleaseResource) override;

	// Handler functions for each asset type
	UFUNCTION()
	void HandleRoadMaterialClick(const FRoadMaterial& RoadMaterial);

	UFUNCTION()
	void HandleBuildingMaterialClick(const FBuildingMaterial& BuildingMaterial);

	UFUNCTION()
	void HandleInteriorMaterialClick(const FInteriorMaterial& InteriorMaterial);

	UFUNCTION()
	void HandleInteriorRoofClick(const FInteriorRoofData& InteriorRoof);

	UFUNCTION()
	void HandleInteriorFloorClick(const FInteriorFloorData& InteriorFloor);

	UFUNCTION()
	void HandleInteriorWallClick(const FInteriorWallData& InteriorWall);

	UFUNCTION()
	void HandleDoorClick(const FDoorData& Door);

	UFUNCTION()
	void HandleWindowClick(const FWindowData& Window);
};

