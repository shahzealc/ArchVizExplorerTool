// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchvizCommonUserWidgetHandler.h"

void UArchvizCommonUserWidgetHandler::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartButton) {
		StartButton->OnClicked.AddDynamic(this, &UArchvizCommonUserWidgetHandler::HandleOnStartEvent);
	}
	if (ConstructionModeCombobox)
	{
		ConstructionModeCombobox->OnSelectionChanged.AddDynamic(this, &UArchvizCommonUserWidgetHandler::HandleConstructionModeChange);
	}
	if (ComboBoxLoadSlotNames)
	{
		ComboBoxLoadSlotNames->OnSelectionChanged.AddDynamic(this, &UArchvizCommonUserWidgetHandler::HandleOnSlotNameChange);
	}
	if (SaveButton) {
		SaveButton->OnClicked.AddDynamic(this, &UArchvizCommonUserWidgetHandler::HandleOnSaveEvent);
	}

	if (LoadButton) {
		LoadButton->OnClicked.AddDynamic(this, &UArchvizCommonUserWidgetHandler::HandleOnLoadEvent);
	}

	if (GuideButton) {
		GuideButton->OnClicked.AddDynamic(this, &UArchvizCommonUserWidgetHandler::HandleOnGuideEvent);
	}
	if (SaveButtonPopUp) {
		SaveButtonPopUp->OnClicked.AddDynamic(this, &UArchvizCommonUserWidgetHandler::HandleOnSaveButtonPopUp);
	}
}

void UArchvizCommonUserWidgetHandler::HandleOnStartEvent()
{
	BlurBackground->SetVisibility(ESlateVisibility::Hidden);
}

void UArchvizCommonUserWidgetHandler::HandleConstructionModeChange(FString SelectedItem, ESelectInfo::Type SelectionType) {
	if (OnConstructionModeChange.IsBound()) {
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Emerald, "Done");

	}
	OnConstructionModeChange.ExecuteIfBound(SelectedItem);
}

void UArchvizCommonUserWidgetHandler::HandleOnSaveButtonPopUp()
{
	PopUpSave->SetVisibility(ESlateVisibility::Visible);
}

void UArchvizCommonUserWidgetHandler::HandleOnSaveEvent()
{
	OnSaveEvent.ExecuteIfBound();
	PopUpSave->SetVisibility(ESlateVisibility::Hidden);
}

void UArchvizCommonUserWidgetHandler::HandleOnLoadEvent()
{
	OnLoadEvent.ExecuteIfBound();
	LoadSwitcher->SetActiveWidget(0);
}

void UArchvizCommonUserWidgetHandler::HandleOnGuideEvent()
{
	OnGuideEvent.ExecuteIfBound();
}

void UArchvizCommonUserWidgetHandler::HandleOnSlotNameChange(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	OnSlotNameChange.ExecuteIfBound(SelectedItem);
	BlurBackground->SetVisibility(ESlateVisibility::Hidden);
}