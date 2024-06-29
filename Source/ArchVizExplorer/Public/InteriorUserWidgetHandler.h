// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/CanvasPanel.h>
#include <Components/ComboBoxString.h>
#include "UniformScrollBoxHandler.h"
#include "InteriorUserWidgetHandler.generated.h"

DECLARE_DELEGATE_OneParam(FOnInteriorFloorClick, FInteriorFloorData);
DECLARE_DELEGATE_OneParam(FOnInteriorRoofClick, FInteriorRoofData);
DECLARE_DELEGATE_OneParam(FOnInteriorWallClick, FInteriorWallData);
/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API UInteriorUserWidgetHandler : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Canvas;

	UPROPERTY(meta = (BindWidget))
	UUniformScrollBoxHandler* InteriorFloorTypes;

	UPROPERTY(meta = (BindWidget))
	UUniformScrollBoxHandler* InteriorRoofTypes;

	UPROPERTY(meta = (BindWidget))
	UUniformScrollBoxHandler* InteriorWallTypes;

	virtual void NativeConstruct() override;

	FOnInteriorFloorClick OnInteriorFloorClick;
	FOnInteriorRoofClick OnInteriorRoofClick;
	FOnInteriorWallClick OnInteriorWallClick;


	UFUNCTION()
	void HandleInteriorFloorClick(const FInteriorFloorData& FloorAsset);

	UFUNCTION()
	void HandleInteriorRoofClick(const FInteriorRoofData& RoofAsset);

	UFUNCTION()
	void HandleInteriorWallClick(const FInteriorWallData& WallAsset);
};
