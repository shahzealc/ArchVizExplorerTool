// Fill out your copyright notice in the Description page of Project Settings.


#include "SUniformScrollBox.h"
#include "SlateOptMacros.h"
#include "Brushes/SlateColorBrush.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SUniformScrollBox::Construct(const FArguments& InArgs)
{
	ScrollContainer = SNew(SScrollBox).Orientation(EOrientation::Orient_Vertical);
	ScrollBoxAssetManager = InArgs._ScrollBoxAssetManager;
	AssetType = InArgs._AssetType;

	TSharedPtr<SVerticalBox> MainVertical = SNew(SVerticalBox);
	AssetTypeText = SNew(STextBlock)
		.Justification(ETextJustify::Center)
		.ColorAndOpacity(FLinearColor(1, 1, 1, 1.0f))
		.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 13));

	MainVertical->AddSlot().FillHeight(0.1)[AssetTypeText.ToSharedRef()];
	MainVertical->AddSlot()[ScrollContainer.ToSharedRef()];

	ChildSlot[MainVertical.ToSharedRef()];

	RefereshAssets();
}

void SUniformScrollBox::RefereshAssets()
{
	ScrollContainer->ClearChildren();

	if (ScrollBoxAssetManager.IsValid())
	{
		UpdateAssetTypeText();
		ShowAssetType();
	}
}

void SUniformScrollBox::UpdateAssetTypeText()
{
	FText AssetText;
	switch (AssetType)
	{
	case EAssetTypes::RoadMaterial:
		AssetText = FText::FromString("Road Materials");
		break;
	case EAssetTypes::BuildingMaterial:
		AssetText = FText::FromString("Building Materials");
		break;
	case EAssetTypes::InteriorFloor:
		AssetText = FText::FromString("Interior Floors");
		break;
	case EAssetTypes::InteriorRoof:
		AssetText = FText::FromString("Interior Roofs");
		break;
	case EAssetTypes::InteriorWall:
		AssetText = FText::FromString("Interior Walls");
		break;
	case EAssetTypes::InteriorMaterial:
		AssetText = FText::FromString("Interior Materials");
		break;
	case EAssetTypes::Door:
		AssetText = FText::FromString("Doors");
		break;
	case EAssetTypes::Window:
		AssetText = FText::FromString("Windows");
		break;
	default:
		AssetText = FText::FromString("Unknown Asset Type");
		break;
	}
	AssetTypeText.ToSharedRef().Get().SetText(AssetText);

}

void SUniformScrollBox::ShowAssetType()
{
	switch (AssetType)
	{
	case EAssetTypes::RoadMaterial:
		ShowRoadMaterial();
		break;
	case EAssetTypes::BuildingMaterial:
		ShowBuildingMaterial();
		break;
	case EAssetTypes::InteriorFloor:
		ShowInteriorFloor();
		break;
	case EAssetTypes::InteriorRoof:
		ShowInteriorRoof();
		break;
	case EAssetTypes::InteriorWall:
		ShowInteriorWall();
		break;
	case EAssetTypes::InteriorMaterial:
		ShowInteriorMaterial();
		break;
	case EAssetTypes::Door:
		ShowDoor();
		break;
	case EAssetTypes::Window:
		ShowWindow();
		break;
	default:
		break;
	}
}


void SUniformScrollBox::ShowRoadMaterial() {

	for (const FRoadMaterial& RoadMaterialData : ScrollBoxAssetManager->RoadMaterial) {

		if (UTexture2D* ThumbnailTexture = Cast<UTexture2D>(RoadMaterialData.RoadImage)) {

			FSlateBrush* ThumbnailBrush = new FSlateBrush();
			ThumbnailBrush->SetResourceObject(ThumbnailTexture);
			ThumbnailBrush->ImageSize = FVector2D(80, 80);

			FSlateColorBrush* BorderBrush = new FSlateColorBrush(FLinearColor(0.651406f, 0.651406f, 0.651406f, 1.0f));

			TSharedPtr<SBorder> BorderAroundImage = SNew(SBorder)
				.BorderImage(BorderBrush)
				.Padding(FMargin(10.0f))
				[
					SNew(SImage)
						.Image(ThumbnailBrush)
						.ToolTipText(FText::FromString(RoadMaterialData.RoadName))
						.OnMouseButtonDown_Lambda([this, RoadMaterialData](const FGeometry& InGeometry, const FPointerEvent& MouseEvent) {
						if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
							CallOnClickRoadMaterial.ExecuteIfBound(RoadMaterialData);
							return FReply::Handled();
						}
						return FReply::Unhandled();
							})
						.Cursor(EMouseCursor::Hand)
				];

			ScrollContainer->AddSlot()
				[
					BorderAroundImage.ToSharedRef()
				];
		}
	}

}

void SUniformScrollBox::ShowBuildingMaterial() {
	for (const FBuildingMaterial& BuildingMaterialData : ScrollBoxAssetManager->BuildingMaterial) {
		if (UTexture2D* ThumbnailTexture = Cast<UTexture2D>(BuildingMaterialData.BuildingMaterialImage)) {
			FSlateBrush* ThumbnailBrush = new FSlateBrush();
			ThumbnailBrush->SetResourceObject(ThumbnailTexture);
			ThumbnailBrush->ImageSize = FVector2D(120, 120);

			FSlateColorBrush* BorderBrush = new FSlateColorBrush(FLinearColor(0.651406f, 0.651406f, 0.651406f, 1.0f));

			TSharedPtr<SBorder> BorderAroundImage = SNew(SBorder)
				.BorderImage(BorderBrush)
				.Padding(FMargin(10.0f))
				[
					SNew(SImage)
						.Image(ThumbnailBrush)
						.ToolTipText(FText::FromString(BuildingMaterialData.BuildingMaterialName))
						.OnMouseButtonDown_Lambda([this, BuildingMaterialData](const FGeometry& InGeometry, const FPointerEvent& MouseEvent) {
						if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
							CallOnClickBuildingMaterial.ExecuteIfBound(BuildingMaterialData);
							return FReply::Handled();
						}
						return FReply::Unhandled();
							})
						.Cursor(EMouseCursor::Hand)
				];

			ScrollContainer->AddSlot()
				[
					BorderAroundImage.ToSharedRef()
				];
		}
	}
}

void SUniformScrollBox::ShowInteriorFloor() {
	for (const FInteriorFloorData& InteriorFloorData : ScrollBoxAssetManager->InteriorFloorData) {
		if (UTexture2D* ThumbnailTexture = Cast<UTexture2D>(InteriorFloorData.InteriorFloorImage)) {
			FSlateBrush* ThumbnailBrush = new FSlateBrush();
			ThumbnailBrush->SetResourceObject(ThumbnailTexture);
			ThumbnailBrush->ImageSize = FVector2D(120, 120);

			FSlateColorBrush* BorderBrush = new FSlateColorBrush(FLinearColor(0.651406f, 0.651406f, 0.651406f, 1.0f));

			TSharedPtr<SBorder> BorderAroundImage = SNew(SBorder)
				.BorderImage(BorderBrush)
				.Padding(FMargin(10.0f))
				[
					SNew(SImage)
						.Image(ThumbnailBrush)
						.ToolTipText(FText::FromString(InteriorFloorData.InteriorFloorName))
						.OnMouseButtonDown_Lambda([this, InteriorFloorData](const FGeometry& InGeometry, const FPointerEvent& MouseEvent) {
						if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
							CallOnClickInteriorFloor.ExecuteIfBound(InteriorFloorData);
							return FReply::Handled();
						}
						return FReply::Unhandled();
							})
						.Cursor(EMouseCursor::Hand)
				];

			ScrollContainer->AddSlot()
				[
					BorderAroundImage.ToSharedRef()
				];
		}
	}
}

void SUniformScrollBox::ShowInteriorRoof() {
	for (const FInteriorRoofData& InteriorRoofData : ScrollBoxAssetManager->InteriorRoofData) {
		if (UTexture2D* ThumbnailTexture = Cast<UTexture2D>(InteriorRoofData.InteriorRoofImage)) {
			FSlateBrush* ThumbnailBrush = new FSlateBrush();
			ThumbnailBrush->SetResourceObject(ThumbnailTexture);
			ThumbnailBrush->ImageSize = FVector2D(120, 120);

			FSlateColorBrush* BorderBrush = new FSlateColorBrush(FLinearColor(0.651406f, 0.651406f, 0.651406f, 1.0f));

			TSharedPtr<SBorder> BorderAroundImage = SNew(SBorder)
				.BorderImage(BorderBrush)
				.Padding(FMargin(10.0f))
				[
					SNew(SImage)
						.Image(ThumbnailBrush)
						.ToolTipText(FText::FromString(InteriorRoofData.InteriorRoofName))
						.OnMouseButtonDown_Lambda([this, InteriorRoofData](const FGeometry& InGeometry, const FPointerEvent& MouseEvent) {
						if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
							CallOnClickInteriorRoof.ExecuteIfBound(InteriorRoofData);
							return FReply::Handled();
						}
						return FReply::Unhandled();
							})
						.Cursor(EMouseCursor::Hand)
				];

			ScrollContainer->AddSlot()
				[
					BorderAroundImage.ToSharedRef()
				];
		}
	}
}

void SUniformScrollBox::ShowInteriorWall() {
	for (const FInteriorWallData& InteriorWallData : ScrollBoxAssetManager->InteriorWallData) {
		if (UTexture2D* ThumbnailTexture = Cast<UTexture2D>(InteriorWallData.InteriorWallImage)) {
			FSlateBrush* ThumbnailBrush = new FSlateBrush();
			ThumbnailBrush->SetResourceObject(ThumbnailTexture);
			ThumbnailBrush->ImageSize = FVector2D(120, 120);

			FSlateColorBrush* BorderBrush = new FSlateColorBrush(FLinearColor(0.651406f, 0.651406f, 0.651406f, 1.0f));

			TSharedPtr<SBorder> BorderAroundImage = SNew(SBorder)
				.BorderImage(BorderBrush)
				.Padding(FMargin(10.0f))
				[
					SNew(SImage)
						.Image(ThumbnailBrush)
						.ToolTipText(FText::FromString(InteriorWallData.InteriorWallName))
						.OnMouseButtonDown_Lambda([this, InteriorWallData](const FGeometry& InGeometry, const FPointerEvent& MouseEvent) {
						if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
							CallOnClickInteriorWall.ExecuteIfBound(InteriorWallData);
							return FReply::Handled();
						}
						return FReply::Unhandled();
							})
						.Cursor(EMouseCursor::Hand)
				];

			TSharedPtr<STextBlock> MeshText = SNew(STextBlock)
				.Text(FText::FromString(InteriorWallData.InteriorWallName))
				.Justification(ETextJustify::Center)
				.ColorAndOpacity(FLinearColor(0.89627f, 0.783538f, 0.40724f, 1.0f))
				.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 20));

			ScrollContainer->AddSlot()
				[
					BorderAroundImage.ToSharedRef()
				];
		}
	}
}

void SUniformScrollBox::ShowInteriorMaterial() {
	for (const FInteriorMaterial& InteriorMaterialData : ScrollBoxAssetManager->InteriorMaterial) {
		if (UTexture2D* ThumbnailTexture = Cast<UTexture2D>(InteriorMaterialData.InteriorMaterialImage)) {
			FSlateBrush* ThumbnailBrush = new FSlateBrush();
			ThumbnailBrush->SetResourceObject(ThumbnailTexture);
			ThumbnailBrush->ImageSize = FVector2D(120, 80);

			FSlateColorBrush* BorderBrush = new FSlateColorBrush(FLinearColor(0.651406f, 0.651406f, 0.651406f, 1.0f));

			TSharedPtr<SBorder> BorderAroundImage = SNew(SBorder)
				.BorderImage(BorderBrush)
				.Padding(FMargin(10.0f))
				[
					SNew(SImage)
						.Image(ThumbnailBrush)
						.OnMouseButtonDown_Lambda([this, InteriorMaterialData](const FGeometry& InGeometry, const FPointerEvent& MouseEvent) {
						if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
							CallOnClickInteriorMaterial.ExecuteIfBound(InteriorMaterialData);
							return FReply::Handled();
						}
						return FReply::Unhandled();
							})
						.Cursor(EMouseCursor::Hand)
				];

			TSharedPtr<STextBlock> MeshText = SNew(STextBlock)
				.Text(FText::FromString(InteriorMaterialData.InteriorMaterialName))
				.Justification(ETextJustify::Center)
				.ColorAndOpacity(FLinearColor(0.89627f, 0.783538f, 0.40724f, 1.0f))
				.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 20));

			ScrollContainer->AddSlot()
				[
					BorderAroundImage.ToSharedRef()
				];
		}
	}
}

void SUniformScrollBox::ShowDoor() {
	for (const FDoorData& DoorData : ScrollBoxAssetManager->DoorData) {
		if (UTexture2D* ThumbnailTexture = Cast<UTexture2D>(DoorData.DoorImage)) {
			FSlateBrush* ThumbnailBrush = new FSlateBrush();
			ThumbnailBrush->SetResourceObject(ThumbnailTexture);
			ThumbnailBrush->ImageSize = FVector2D(80, 120);

			FSlateColorBrush* BorderBrush = new FSlateColorBrush(FLinearColor(0.651406f, 0.651406f, 0.651406f, 1.0f));

			TSharedPtr<SBorder> BorderAroundImage = SNew(SBorder)
				.BorderImage(BorderBrush)
				.Padding(FMargin(10.0f))
				[
					SNew(SImage)
						.Image(ThumbnailBrush)
						.ToolTipText(FText::FromString(DoorData.DoorName))
						.OnMouseButtonDown_Lambda([this, DoorData](const FGeometry& InGeometry, const FPointerEvent& MouseEvent) {
						if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
							CallOnClickDoor.ExecuteIfBound(DoorData);
							return FReply::Handled();
						}
						return FReply::Unhandled();
							})
						.Cursor(EMouseCursor::Hand)
				];

			ScrollContainer->AddSlot()
				[
					BorderAroundImage.ToSharedRef()
				];
		}
	}
}

void SUniformScrollBox::ShowWindow() {
	for (const FWindowData& WindowData : ScrollBoxAssetManager->WindowData) {
		if (UTexture2D* ThumbnailTexture = Cast<UTexture2D>(WindowData.WindowImage)) {
			FSlateBrush* ThumbnailBrush = new FSlateBrush();
			ThumbnailBrush->SetResourceObject(ThumbnailTexture);
			ThumbnailBrush->ImageSize = FVector2D(80, 120);

			FSlateColorBrush* BorderBrush = new FSlateColorBrush(FLinearColor(0.651406f, 0.651406f, 0.651406f, 1.0f));

			TSharedPtr<SBorder> BorderAroundImage = SNew(SBorder)
				.BorderImage(BorderBrush)
				.Padding(FMargin(10.0f))
				[
					SNew(SImage)
						.Image(ThumbnailBrush)
						.ToolTipText(FText::FromString(WindowData.WindowName))
						.OnMouseButtonDown_Lambda([this, WindowData](const FGeometry& InGeometry, const FPointerEvent& MouseEvent) {
						if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
							CallOnClickWindow.ExecuteIfBound(WindowData);
							return FReply::Handled();
						}
						return FReply::Unhandled();
							})
						.Cursor(EMouseCursor::Hand)
				];

			ScrollContainer->AddSlot()
				[
					BorderAroundImage.ToSharedRef()
				];
		}
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
