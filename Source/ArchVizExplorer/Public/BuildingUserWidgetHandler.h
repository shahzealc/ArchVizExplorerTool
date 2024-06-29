// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/CanvasPanel.h>

#include <Components/ComboBoxString.h>
#include <Components/EditableText.h>
#include <Components/UniformGridPanel.h>
#include <Components/SpinBox.h>
#include <Components/VerticalBox.h>
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include "UniformScrollBoxHandler.h"
#include "BuildingUserWidgetHandler.generated.h"

DECLARE_DELEGATE_OneParam(FOnBuildingModeChange, FString);
DECLARE_DELEGATE_OneParam(FOnSegmentChange, int);
DECLARE_DELEGATE_OneParam(FOnDoorClick, FDoorData);
DECLARE_DELEGATE_OneParam(FOnWindowClick, FWindowData);
DECLARE_DELEGATE(FOnRoofFloorButtonClick);
DECLARE_DELEGATE(FOnIncrementStoreyButtonClick);
DECLARE_DELEGATE(FOnDecrementStoreyButtonClick);

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API UBuildingUserWidgetHandler : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int CurrentStorey = 0;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Canvas;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* BuildingConstructionMode;

	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* SegmentUniformPanel;

	UPROPERTY(meta = (BindWidget))
	USpinBox* WallSegment;

	UPROPERTY(meta = (BindWidget))
	UButton* RoofFloorButton;

	UPROPERTY(meta = (BindWidget))
	UButton* IncrementStoreyButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentStoreyText;

	UPROPERTY(meta = (BindWidget))
	UButton* DecrementStoreyButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RoofFloorText;

	UPROPERTY(meta = (BindWidget))
	UUniformScrollBoxHandler* DoorTypes;

	UPROPERTY(meta = (BindWidget))
	UUniformScrollBoxHandler* WindowTypes;

	FOnSegmentChange OnSegmentChange;
	FOnDoorClick OnDoorClick;
	FOnWindowClick OnWindowClick;
	FOnBuildingModeChange OnBuildingModeChange;
	FOnRoofFloorButtonClick OnRoofFloorButtonClick;
	FOnIncrementStoreyButtonClick OnIncrementStoreyButtonClick;
	FOnDecrementStoreyButtonClick OnDecrementStoreyButtonClick;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleOnSegmentChange(float value);

	UFUNCTION()
	void HandleRoofFloorButtonClick();

	UFUNCTION()
	void HandleIncrementStoreyButtonClick();

	UFUNCTION()
	void HandleDecrementStoreyButtonClick();

	UFUNCTION()
	void SetStoreyText();

	UFUNCTION()
	void HandleBuildingModeChange(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void HandleDoorClick(const FDoorData& DoorAsset);

	UFUNCTION()
	void HandleWindowClick(const FWindowData& WindowAsset);
};
