// Fill out your copyright notice in the Description page of Project Settings.


#include "InteriorUserWidgetHandler.h"

void UInteriorUserWidgetHandler::NativeConstruct()
{
	Super::NativeConstruct();

	if (InteriorRoofTypes) {
		InteriorRoofTypes->OnClickInteriorRoof.BindUObject(this, &UInteriorUserWidgetHandler::HandleInteriorRoofClick);
	}
	if (InteriorFloorTypes) {
		InteriorFloorTypes->OnClickInteriorFloor.BindUObject(this, &UInteriorUserWidgetHandler::HandleInteriorFloorClick);
	}
	if (InteriorWallTypes) {
		InteriorWallTypes->OnClickInteriorWall.BindUObject(this, &UInteriorUserWidgetHandler::HandleInteriorWallClick);
	}

}

void UInteriorUserWidgetHandler::HandleInteriorFloorClick(const FInteriorFloorData& FloorAsset)
{
	OnInteriorFloorClick.ExecuteIfBound(FloorAsset);
}

void UInteriorUserWidgetHandler::HandleInteriorRoofClick(const FInteriorRoofData& RoofAsset)
{
	OnInteriorRoofClick.ExecuteIfBound(RoofAsset);
}

void UInteriorUserWidgetHandler::HandleInteriorWallClick(const FInteriorWallData& WallAsset)
{
	OnInteriorWallClick.ExecuteIfBound(WallAsset);
}
