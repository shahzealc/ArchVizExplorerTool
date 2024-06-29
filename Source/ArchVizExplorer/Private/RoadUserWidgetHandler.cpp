// Fill out your copyright notice in the Description page of Project Settings.


#include "RoadUserWidgetHandler.h"
#include <Components/Image.h>
#include <Components/ButtonSlot.h>
#include <Components/SizeBox.h>

void URoadUserWidgetHandler::NativeConstruct()
{
	Super::NativeConstruct();

	if (WidthSpinBox) {
		WidthSpinBox->OnValueChanged.AddDynamic(this, &URoadUserWidgetHandler::HandleOnWidthChange);
	}
	
	if (RoadMaterial) {
		RoadMaterial->OnClickRoadMaterial.BindUObject(this, &URoadUserWidgetHandler::HandleRoadMaterial);
	}

}

void URoadUserWidgetHandler::HandleOnWidthChange(float value)
{
	if (OnWidthChange.IsBound()) {
		OnWidthChange.Execute(value);
	}
}

void URoadUserWidgetHandler::HandleRoadMaterial(const FRoadMaterial& RoadAsset) {
	OnRoadMaterial.ExecuteIfBound(RoadAsset);
}