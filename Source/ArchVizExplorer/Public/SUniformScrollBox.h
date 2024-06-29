// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Widgets/Layout/SScrollBox.h>
#include "ScrollBoxAssetManager.h"
#include "Widgets/SCompoundWidget.h"

DECLARE_DELEGATE_OneParam(FCallOnClickRoadMaterial, const FRoadMaterial&);
DECLARE_DELEGATE_OneParam(FCallOnClickBuildingMaterial, const FBuildingMaterial&);
DECLARE_DELEGATE_OneParam(FCallOnClickInteriorFloor, const FInteriorFloorData&);
DECLARE_DELEGATE_OneParam(FCallOnClickInteriorRoof, const FInteriorRoofData&);
DECLARE_DELEGATE_OneParam(FCallOnClickInteriorWall, const FInteriorWallData&);
DECLARE_DELEGATE_OneParam(FCallOnClickInteriorMaterial, const FInteriorMaterial&);
DECLARE_DELEGATE_OneParam(FCallOnClickDoor, const FDoorData&);
DECLARE_DELEGATE_OneParam(FCallOnClickWindow, const FWindowData&);


UENUM(BlueprintType)
enum class EAssetTypes : uint8
{
	RoadMaterial,
	BuildingMaterial,
	InteriorFloor,
	InteriorRoof,
	InteriorWall,
	InteriorMaterial,
	Door,
	Window
};

class ARCHVIZEXPLORER_API SUniformScrollBox : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SUniformScrollBox)
	{}
		SLATE_ARGUMENT(TWeakObjectPtr<UScrollBoxAssetManager>, ScrollBoxAssetManager)
		SLATE_ARGUMENT(EAssetTypes, AssetType)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	FCallOnClickRoadMaterial CallOnClickRoadMaterial;
	FCallOnClickBuildingMaterial CallOnClickBuildingMaterial;
	FCallOnClickInteriorFloor CallOnClickInteriorFloor;
	FCallOnClickInteriorRoof CallOnClickInteriorRoof;
	FCallOnClickInteriorWall CallOnClickInteriorWall;
	FCallOnClickInteriorMaterial CallOnClickInteriorMaterial;
	FCallOnClickDoor CallOnClickDoor;
	FCallOnClickWindow CallOnClickWindow;

private:

	TWeakObjectPtr<UScrollBoxAssetManager> ScrollBoxAssetManager;

	TSharedPtr<SScrollBox> ScrollContainer;
	TSharedPtr <STextBlock> AssetTypeText;
	TEnumAsByte<EAssetTypes> AssetType;

	void RefereshAssets();

	void UpdateAssetTypeText();

	void ShowAssetType();

	void ShowRoadMaterial();
	void ShowBuildingMaterial();
	void ShowInteriorFloor();
	void ShowInteriorRoof();
	void ShowInteriorWall();
	void ShowInteriorMaterial();
	void ShowDoor();
	void ShowWindow();

};
