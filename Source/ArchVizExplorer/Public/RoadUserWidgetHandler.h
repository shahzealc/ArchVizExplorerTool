// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/CanvasPanel.h>

#include <Components/TextBlock.h>
#include <Components/UniformGridPanel.h>
#include <Components/SpinBox.h>
#include <Components/Button.h>
#include <Components/ScrollBox.h>
#include <Components/ComboBoxString.h>
#include "UniformScrollBoxHandler.h"
#include "ScrollBoxAssetManager.h"
#include "RoadUserWidgetHandler.generated.h"

DECLARE_DELEGATE_OneParam(FOnRoadMaterial, FRoadMaterial);
DECLARE_DELEGATE_OneParam(FOnWidthChange, float);

UCLASS()
class ARCHVIZEXPLORER_API URoadUserWidgetHandler : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Canvas;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WidthText;

	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* WidthUniformPanel;

	UPROPERTY(meta = (BindWidget))
	USpinBox* WidthSpinBox;

	UPROPERTY(meta = (BindWidget))
	UUniformScrollBoxHandler* RoadMaterial;

	UPROPERTY()
	bool bIsRoadConstruction = true;

	FOnWidthChange OnWidthChange;

	FOnRoadMaterial OnRoadMaterial;

	virtual void NativeConstruct() override;

private:

	UFUNCTION()
	void HandleOnWidthChange(float value);

	UFUNCTION()
	void HandleRoadMaterial(const FRoadMaterial& RoadAsset);

};
