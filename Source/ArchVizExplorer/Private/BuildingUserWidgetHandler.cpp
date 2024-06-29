// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingUserWidgetHandler.h"

void UBuildingUserWidgetHandler::NativeConstruct()
{
	Super::NativeConstruct();

	if (WallSegment) {
		WallSegment->OnValueChanged.AddDynamic(this, &UBuildingUserWidgetHandler::HandleOnSegmentChange);
	}
	if (RoofFloorButton) {
		RoofFloorButton->OnClicked.AddDynamic(this, &UBuildingUserWidgetHandler::HandleRoofFloorButtonClick);
	}
	if (BuildingConstructionMode) {
		BuildingConstructionMode->OnSelectionChanged.AddDynamic(this, &UBuildingUserWidgetHandler::HandleBuildingModeChange);
	}
	if (DoorTypes) {
		DoorTypes->OnClickDoor.BindUObject(this, &UBuildingUserWidgetHandler::HandleDoorClick);
	}
	if (WindowTypes) {
		WindowTypes->OnClickWindow.BindUObject(this, &UBuildingUserWidgetHandler::HandleWindowClick);
	}
	if (IncrementStoreyButton) {
		IncrementStoreyButton->OnClicked.AddDynamic(this, &UBuildingUserWidgetHandler::HandleIncrementStoreyButtonClick);
	}
	if (DecrementStoreyButton) {
		DecrementStoreyButton->OnClicked.AddDynamic(this, &UBuildingUserWidgetHandler::HandleDecrementStoreyButtonClick);
	}
	


}

void UBuildingUserWidgetHandler::HandleOnSegmentChange(float value) {

	OnSegmentChange.ExecuteIfBound(value);
}

void UBuildingUserWidgetHandler::HandleRoofFloorButtonClick()
{
	OnRoofFloorButtonClick.ExecuteIfBound();
}

void UBuildingUserWidgetHandler::HandleBuildingModeChange(FString SelectedItem, ESelectInfo::Type SelectionType) {
	OnBuildingModeChange.ExecuteIfBound(SelectedItem);
}

void UBuildingUserWidgetHandler::HandleDoorClick(const FDoorData& DoorAsset)
{
	OnDoorClick.ExecuteIfBound(DoorAsset);
}

void UBuildingUserWidgetHandler::HandleWindowClick(const FWindowData& WindowAsset)
{
	OnWindowClick.ExecuteIfBound(WindowAsset);
}


void UBuildingUserWidgetHandler::HandleIncrementStoreyButtonClick()
{
	OnIncrementStoreyButtonClick.ExecuteIfBound();
	SetStoreyText();
}

void UBuildingUserWidgetHandler::HandleDecrementStoreyButtonClick()
{
	if (CurrentStorey > 0) {
		--CurrentStorey;
	}
	SetStoreyText();

}

void UBuildingUserWidgetHandler::SetStoreyText() {
	if (CurrentStoreyText) {

		FText StoreyText;
		if (CurrentStorey == 0) {
			StoreyText = FText::FromString("0 : Ground Floor");
		}
		else {
			StoreyText = FText::FromString(FString::FromInt(CurrentStorey) + FString(" : New Storey"));
		}

		CurrentStoreyText->SetText(StoreyText);
	}
}