// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "RoadActor.h"
#include "RoadUserWidgetHandler.h"
#include "WallActor.h"
#include "InteriorActor.h"
#include "BuildingUserWidgetHandler.h"
#include "InteriorUserWidgetHandler.h"
#include "MaterialUserWidgetHandler.h"
#include "RoofAndFloorActor.h"
#include "ArchVizSaveGame.h"
#include <Components/SplineComponent.h>
#include "ArchvizCommonUserWidgetHandler.h"
#include "TimerManager.h"
#include "ArchVizSavedSlotName.h"
#include "ArchVizController.generated.h"


UENUM()
enum EConstructionMode
{
	Road,
	Building,
	Interior,
	Material,
	View
};

UENUM()
enum EBuildingMode
{
	Wall,
	Roof,
	Floor,
	Door,
	Window
};

UENUM()
enum EInteriorType
{
	None,
	InteriorRoof,
	InteriorFloor,
	InteriorWall
};

UENUM()
enum EMaterialType
{
	WallMaterial,
	RoofFloorMaterial,
	RoadMaterial
};

UCLASS()
class ARCHVIZEXPLORER_API AArchVizController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupInputComponent() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common Widget")
	TSubclassOf<UArchvizCommonUserWidgetHandler> CommonUserWidgetHandlerClass;

	UPROPERTY()
	UArchvizCommonUserWidgetHandler* CommonUserWidgetHandler;

	//Save And Load
	UFUNCTION()
	void SaveArchviz();

	UFUNCTION()
	void LoadArchviz(FString SlotNameToLoad);

	UFUNCTION()
	void LoadArchvizSlotNames();

	UPROPERTY()
	TArray<FString> SavedSlotNames;

	//Save Load End

	UPROPERTY()
	TEnumAsByte<EConstructionMode> ConstructionMode;

	UFUNCTION()
	void ChangeConstructionMode(FString Mode);

	//Road Construction
	UPROPERTY()
	class UInputMappingContext* RoadBuilderMappingContext;

	UPROPERTY()
	class UInputAction* RoadBuilderMouseLeftClick;

	UPROPERTY()
	class UInputAction* RoadBuilderMouseRightClick;

	UPROPERTY()
	class UInputAction* RoadBuilderDeleteKey;

	UPROPERTY()
	class UInputAction* RoadBuilderMiddleMouse;

	UPROPERTY()
	FVector FirstClickLocation;

	UPROPERTY()
	FVector SecondClickLocation;

	UPROPERTY()
	bool bFirstClick = true;

	UPROPERTY()
	bool bFirstRoad = true;

	UPROPERTY()
	ARoadActor* RoadActor;

	UPROPERTY()
	ARoadActor* CurrentSelectedRoadActor;

	UPROPERTY()
	float Length;

	UPROPERTY()
	float Width = 300;

	UPROPERTY()
	float Height = 5;

	UPROPERTY()
	FVector PreviousRoadDirection;

	UPROPERTY()
	FVector PreviousClickLocation;

	UPROPERTY()
	bool bHasPreviousRoad = false;

	UPROPERTY()
	FRotator PreviousRoadRotation;

	UPROPERTY(EditAnywhere, Category = "Road Material")
	UMaterialInstance* DefaultRoadMaterial;

	UFUNCTION()
	void ChangeRoadWidth(float width_);

	UFUNCTION()
	void ChangeRoadMaterial(const FRoadMaterial& RoadMaterial_);

	UFUNCTION()
	void DestroyRoad();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Road Widget")
	TSubclassOf<URoadUserWidgetHandler> RoadUserWidgetHandlerClass;

	UPROPERTY()
	URoadUserWidgetHandler* RoadUserWidgetHandler;

	UPROPERTY(EditAnywhere, Category = "Selection Material")
	UMaterial* SelectionMaterial;

	//Building Construction

	UPROPERTY()
	TEnumAsByte<EBuildingMode> BuildingMode;

	UPROPERTY()
	class UInputMappingContext* BuildingMappingContext;

	UPROPERTY()
	class UInputAction* BuildingMouseLeftClick;

	UPROPERTY()
	class UInputAction* BuildingMouseRightClick;

	UPROPERTY()
	class UInputAction* BuildingMiddleMouseButton;

	UPROPERTY()
	class UInputAction* BuildingRotateWall;

	UPROPERTY()
	class UInputAction* BuildingDeleteWall;


	UPROPERTY()
	AWallActor* WallActor;

	UPROPERTY()
	bool bMoveWall = false;

	UPROPERTY()
	int WallSnapValue = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Widget")
	TSubclassOf<UBuildingUserWidgetHandler> BuildingUserWidgetHandlerClass;

	UPROPERTY()
	UBuildingUserWidgetHandler* BuildingUserWidgetHandler;

	UFUNCTION()
	void ChangeSegment(float Segments);

	UFUNCTION()
	void PlaceDoor(const FDoorData& DoorData_);

	UFUNCTION()
	void PlaceWindow(const FWindowData& WindowData_);

	UFUNCTION()
	void IncrementStoreyCheck();

	UPROPERTY()
	ARoofAndFloorActor* RoofFloor;

	UPROPERTY()
	ARoofAndFloorActor* SelectedFloorActor;

	UPROPERTY()
	ARoofAndFloorActor* SelectedRoofActor;

	UFUNCTION()
	void PlaceRoofFloor();

	UFUNCTION()
	void ChangeBuildingMode(FString Mode);

	UPROPERTY()
	UStaticMeshComponent* SelectedWallMesh;

	UPROPERTY()
	UStaticMeshComponent* SelectedDoorMesh;

	UPROPERTY()
	UStaticMeshComponent* SelectedWindowMesh;

	UPROPERTY()
	FRotator SelectedWallMeshRotation;

	UPROPERTY()
	TArray<UStaticMeshComponent*> SelectedWallStaticMesh;

	UPROPERTY(EditAnywhere, Category = "Wall Material")
	UMaterialInstance* WallMaterial;

	UPROPERTY()
	TArray<FVector> RoofLocations;

	UPROPERTY()
	TArray<FVector> FloorLocations;

	UPROPERTY()
	TArray<ARoofAndFloorActor*> OverviewActors;

	UFUNCTION()
	void DestroyWall();

	//Interior Design

	UPROPERTY()
	TEnumAsByte<EInteriorType> InteriorType;

	UPROPERTY()
	class UInputMappingContext* InteriorMappingContext;

	UPROPERTY()
	class UInputAction* InteriorMouseLeftClick;

	UPROPERTY()
	class UInputAction* InteriorMouseMiddleClick;

	UPROPERTY()
	class UInputAction* InteriorRotate;

	UPROPERTY()
	class UInputAction* InteriorDelete;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interior Widget")
	TSubclassOf<UInteriorUserWidgetHandler> InteriorUserWidgetHandlerClass;

	UPROPERTY()
	UInteriorUserWidgetHandler* InteriorUserWidgetHandler;

	UPROPERTY()
	AInteriorActor* InteriorActor;

	UPROPERTY()
	bool bMoveInterior = false;

	UFUNCTION()
	void RenderInteriorFloor(const FInteriorFloorData& FloorData_);

	UFUNCTION()
	void RenderInteriorRoof(const FInteriorRoofData& RoofData_);

	UFUNCTION()
	void RenderInteriorWall(const FInteriorWallData& WallData_);

	UPROPERTY()
	TArray<UStaticMeshComponent*> SelectedInteriorStaticMesh;

	UFUNCTION()
	void DestroyInterior();

	//Material Selection

	TEnumAsByte<EMaterialType> MaterialType;

	UPROPERTY()
	class UInputMappingContext* MaterialMappingContext;

	UPROPERTY()
	class UInputAction* MaterialMouseLeftClick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material Widget")
	TSubclassOf<UMaterialUserWidgetHandler> MaterialUserWidgetHandlerClass;

	UPROPERTY()
	UMaterialUserWidgetHandler* MaterialUserWidgetHandler;

	UFUNCTION()
	void ApplyBuildingMaterial(const FBuildingMaterial& BuildingMaterial);

	UFUNCTION()
	void ApplyRoadMaterial(const FRoadMaterial& RoadMaterial);

	//

	UFUNCTION()
	void CleanUp();

	UPROPERTY()
	FTimerHandle Timehandle;

	UFUNCTION()
	void ShowModeSpecificInstruction();

private:

	void HideInstructionText();

	void ClearAllModesMapping();

	//Road Construction
	void RoadBuilderMouseLeftClickLocation();
	void RoadBuilderNewRoadCreation();
	void SelectRoadActor();
	void SetUpRoadInputComponent();

	//Building Construction
	void SetUpBuildingInputComponent();
	void BuildingLeftMouseLogic();
	void BuildingMiddleMouseLogic();
	void BuildingRightMouseLogic();
	void PlaceWall();
	void NewWall();
	void RotateWall();
	void SnapWall();
	void SelectWallSegment();
	void SelectDoor();
	void SelectWindow();
	void SelectWallActor();
	void SelectFloorActor();
	void SelectRoofActor();
	void StoreFloorPoints();
	void StoreRoofPoints();
	void DestroyOverView();
	void StoreRoofOrFloorPoints(TArray<FVector>& Locations, bool bIsRoof);
	void DrawTemporaryLine(FVector Start, FVector End, bool bIsRoof);
	void DisableRendering(TArray<UStaticMeshComponent*>);

	//Interior Design
	void SetUpInteriorInputComponent();
	void InteriorLeftMouseLogic();
	bool CheckInteriorCollision(const FVector& Location, const FVector& ActorExtent, AInteriorActor* InteriorActorIgnore);
	FVector GetActorExtentFromMesh(UStaticMesh* Mesh);
	void SelectInteriorActor();
	void RotateInterior();

	//Material Selection
	void SetUpMaterialInputComponent();
	void MaterialLeftMouseLogic();

	//
	void SetInstructionText(FString Text, int time = 2);

	//
	FVector GridOrigin;
	float GridSize = 100.0f; // Adjust based on your desired road length
	TArray<FVector> PlacedRoadEnds;

	FVector SnapToGrid(const FVector& Location);
	FVector FindNearestRoadEnd(const FVector& Location);
	void PlaceRoadSegment(const FVector& Start, const FVector& Direction);
	void PlaceCornerPiece(const FVector& Location, const FVector& Direction);
};
