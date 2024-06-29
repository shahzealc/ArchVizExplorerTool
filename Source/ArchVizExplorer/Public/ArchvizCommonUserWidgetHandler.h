// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/ComboBoxString.h>
#include <Components/CanvasPanel.h>
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include <Components/WidgetSwitcher.h>
#include <Components/Border.h>
#include <Components/EditableText.h>
#include <Components/BackgroundBlur.h>
#include "ArchvizCommonUserWidgetHandler.generated.h"

DECLARE_DELEGATE_OneParam(FOnConstructionModeChange, FString);
DECLARE_DELEGATE_OneParam(FOnSlotNameChange, FString);
DECLARE_DELEGATE(FOnStartEvent);
DECLARE_DELEGATE(FOnSaveEvent);
DECLARE_DELEGATE(FOnLoadEvent);
DECLARE_DELEGATE(FOnGuideEvent);

UCLASS()
class ARCHVIZEXPLORER_API UArchvizCommonUserWidgetHandler : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Canvas;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ConstructionModeCombobox;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ComboBoxLoadSlotNames;

	UPROPERTY(meta = (BindWidget))
	UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveButtonPopUp;

	UPROPERTY(meta = (BindWidget))
	UButton* LoadButton;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* LoadSwitcher;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* InstructionText;

	UPROPERTY(meta = (BindWidget))
	UButton* GuideButton;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* InstructionSwitcher;

	UPROPERTY(meta = (BindWidget))
	UBackgroundBlur* BlurBackground;

	UPROPERTY(meta = (BindWidget))
	UBorder* PopUpSave;

	UPROPERTY(meta = (BindWidget))
	UEditableText* FileNameText;

	virtual void NativeConstruct() override;

	FOnConstructionModeChange OnConstructionModeChange;
	FOnSaveEvent OnSaveEvent;
	FOnLoadEvent OnLoadEvent;
	FOnGuideEvent OnGuideEvent;
	FOnSlotNameChange OnSlotNameChange;
	FOnStartEvent OnStartEvent;

	UFUNCTION()
	void HandleConstructionModeChange(FString SelectedItem, ESelectInfo::Type SelectionType);
	
	UFUNCTION()
	void HandleOnStartEvent();

	UFUNCTION()
	void HandleOnSaveEvent();

	UFUNCTION()
	void HandleOnLoadEvent();

	UFUNCTION()
	void HandleOnGuideEvent();

	UFUNCTION()
	void HandleOnSaveButtonPopUp();

	UFUNCTION()
	void HandleOnSlotNameChange(FString SelectedItem, ESelectInfo::Type SelectionType);
};
