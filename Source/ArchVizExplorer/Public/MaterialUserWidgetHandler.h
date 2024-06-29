// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/CanvasPanel.h>
#include <Components/ComboBoxString.h>
#include "UniformScrollBoxHandler.h"
#include "MaterialUserWidgetHandler.generated.h"

DECLARE_DELEGATE_OneParam(FOnBuildingMaterialClick, FBuildingMaterial);
DECLARE_DELEGATE_OneParam(FOnRoadMaterialClick, FRoadMaterial);

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORER_API UMaterialUserWidgetHandler : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Canvas;

	UPROPERTY(meta = (BindWidget))
	UUniformScrollBoxHandler* BuildingMaterialContainer;

	UPROPERTY(meta = (BindWidget))
	UUniformScrollBoxHandler* RoadMaterialContainer;

	virtual void NativeConstruct() override;

	FOnBuildingMaterialClick OnBuildingMaterialClick;
	FOnRoadMaterialClick OnRoadMaterialClick;

	UFUNCTION()
	void HandleBuildingMaterialClick(const FBuildingMaterial& BuildingMaterial);

	UFUNCTION()
	void HandleRoadMaterialClick(const FRoadMaterial& RoadMaterial);
};
