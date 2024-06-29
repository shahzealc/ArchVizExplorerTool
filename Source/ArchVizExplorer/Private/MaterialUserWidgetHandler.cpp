// Fill out your copyright notice in the Description page of Project Settings.


#include "MaterialUserWidgetHandler.h"

void UMaterialUserWidgetHandler::NativeConstruct()
{
	Super::NativeConstruct();

	if (BuildingMaterialContainer) {
		BuildingMaterialContainer->OnClickBuildingMaterial.BindUObject(this, &UMaterialUserWidgetHandler::HandleBuildingMaterialClick);
	}
	if (RoadMaterialContainer) {
		RoadMaterialContainer->OnClickRoadMaterial.BindUObject(this, &UMaterialUserWidgetHandler::HandleRoadMaterialClick);
	}
}


void UMaterialUserWidgetHandler::HandleBuildingMaterialClick(const FBuildingMaterial& BuildingMaterial)
{	
	OnBuildingMaterialClick.ExecuteIfBound(BuildingMaterial);
}

void UMaterialUserWidgetHandler::HandleRoadMaterialClick(const FRoadMaterial& RoadMaterial)
{
	OnRoadMaterialClick.ExecuteIfBound(RoadMaterial);
}
