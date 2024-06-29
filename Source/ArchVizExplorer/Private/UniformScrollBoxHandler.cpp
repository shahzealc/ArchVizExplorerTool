// Fill out your copyright notice in the Description page of Project Settings.


#include "UniformScrollBoxHandler.h"

TSharedRef<SWidget> UUniformScrollBoxHandler::RebuildWidget()
{
    UniformScrollBoxSlate = SNew(SUniformScrollBox)
        .ScrollBoxAssetManager(ScrollBoxAssetManager)
        .AssetType(AssetType);

    switch (AssetType)
    {
    case EAssetTypes::RoadMaterial:
        UniformScrollBoxSlate->CallOnClickRoadMaterial.BindUFunction(this, "HandleRoadMaterialClick");
        break;
    case EAssetTypes::BuildingMaterial:
        UniformScrollBoxSlate->CallOnClickBuildingMaterial.BindUFunction(this, "HandleBuildingMaterialClick");
        break;
    case EAssetTypes::InteriorMaterial:
        UniformScrollBoxSlate->CallOnClickInteriorMaterial.BindUFunction(this, "HandleInteriorMaterialClick");
        break;
    case EAssetTypes::InteriorRoof:
        UniformScrollBoxSlate->CallOnClickInteriorRoof.BindUFunction(this, "HandleInteriorRoofClick");
        break;
    case EAssetTypes::InteriorFloor:
        UniformScrollBoxSlate->CallOnClickInteriorFloor.BindUFunction(this, "HandleInteriorFloorClick");
        break;
    case EAssetTypes::InteriorWall:
        UniformScrollBoxSlate->CallOnClickInteriorWall.BindUFunction(this, "HandleInteriorWallClick");
        break;
    case EAssetTypes::Door:
        UniformScrollBoxSlate->CallOnClickDoor.BindUFunction(this, "HandleDoorClick");
        break;
    case EAssetTypes::Window:
        UniformScrollBoxSlate->CallOnClickWindow.BindUFunction(this, "HandleWindowClick");
        break;
    }

    return UniformScrollBoxSlate.ToSharedRef();
}

const FText UUniformScrollBoxHandler::GetPaletteCategory() {
	return FText::FromString("Panel");
}

void UUniformScrollBoxHandler::ReleaseSlateResources(bool ReleaseResource)
{
	Super::ReleaseSlateResources(ReleaseResource);
	if (UniformScrollBoxSlate.IsValid()) {
		UniformScrollBoxSlate.Reset();
	}
}

void UUniformScrollBoxHandler::HandleRoadMaterialClick(const FRoadMaterial& RoadMaterial)
{
	OnClickRoadMaterial.ExecuteIfBound(RoadMaterial);
}

void UUniformScrollBoxHandler::HandleBuildingMaterialClick(const FBuildingMaterial& BuildingMaterial)
{
	OnClickBuildingMaterial.ExecuteIfBound(BuildingMaterial);
}

void UUniformScrollBoxHandler::HandleInteriorMaterialClick(const FInteriorMaterial& InteriorMaterial)
{
	OnClickInteriorMaterial.ExecuteIfBound(InteriorMaterial);
}

void UUniformScrollBoxHandler::HandleInteriorRoofClick(const FInteriorRoofData& InteriorRoof)
{
	OnClickInteriorRoof.ExecuteIfBound(InteriorRoof);
}

void UUniformScrollBoxHandler::HandleInteriorFloorClick(const FInteriorFloorData& InteriorFloor)
{
	OnClickInteriorFloor.ExecuteIfBound(InteriorFloor);
}

void UUniformScrollBoxHandler::HandleInteriorWallClick(const FInteriorWallData& InteriorWall)
{
	OnClickInteriorWall.ExecuteIfBound(InteriorWall);
}

void UUniformScrollBoxHandler::HandleDoorClick(const FDoorData& Door)
{
	OnClickDoor.ExecuteIfBound(Door);
}

void UUniformScrollBoxHandler::HandleWindowClick(const FWindowData& Window)
{
	OnClickWindow.ExecuteIfBound(Window);
}

