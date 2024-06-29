// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizController.h"
#include <Kismet/KismetMathLibrary.h>
#include <EngineUtils.h>
#include <Kismet/GameplayStatics.h>

void AArchVizController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(true);

	if (CommonUserWidgetHandlerClass) {
		CommonUserWidgetHandler = CreateWidget<UArchvizCommonUserWidgetHandler>(this, CommonUserWidgetHandlerClass);
		CommonUserWidgetHandler->AddToViewport();
		CommonUserWidgetHandler->OnConstructionModeChange.BindUFunction(this, FName("ChangeConstructionMode"));
		CommonUserWidgetHandler->OnSaveEvent.BindUFunction(this, FName("SaveArchviz"));
		CommonUserWidgetHandler->OnLoadEvent.BindUFunction(this, FName("LoadArchvizSlotNames"));
		CommonUserWidgetHandler->OnGuideEvent.BindUFunction(this, FName("ShowModeSpecificInstruction"));
		CommonUserWidgetHandler->OnSlotNameChange.BindUFunction(this, FName("LoadArchviz"));
	}
	if (RoadUserWidgetHandlerClass) {
		RoadUserWidgetHandler = CreateWidget<URoadUserWidgetHandler>(this, RoadUserWidgetHandlerClass);
		RoadUserWidgetHandler->AddToViewport();
		RoadUserWidgetHandler->OnWidthChange.BindUFunction(this, FName("ChangeRoadWidth"));
		RoadUserWidgetHandler->OnRoadMaterial.BindUFunction(this, FName("ChangeRoadMaterial"));
	}
	if (BuildingUserWidgetHandlerClass) {
		BuildingUserWidgetHandler = CreateWidget<UBuildingUserWidgetHandler>(this, BuildingUserWidgetHandlerClass);
		BuildingUserWidgetHandler->OnBuildingModeChange.BindUFunction(this, FName("ChangeBuildingMode"));
		BuildingUserWidgetHandler->OnSegmentChange.BindUFunction(this, FName("ChangeSegment"));
		BuildingUserWidgetHandler->OnDoorClick.BindUFunction(this, FName("PlaceDoor"));
		BuildingUserWidgetHandler->OnWindowClick.BindUFunction(this, FName("PlaceWindow"));
		BuildingUserWidgetHandler->OnRoofFloorButtonClick.BindUFunction(this, FName("PlaceRoofFloor"));
		BuildingUserWidgetHandler->OnIncrementStoreyButtonClick.BindUFunction(this, FName("IncrementStoreyCheck"));
	}
	if (InteriorUserWidgetHandlerClass) {
		InteriorUserWidgetHandler = CreateWidget<UInteriorUserWidgetHandler>(this, InteriorUserWidgetHandlerClass);
		InteriorUserWidgetHandler->OnInteriorFloorClick.BindUFunction(this, FName("RenderInteriorFloor"));
		InteriorUserWidgetHandler->OnInteriorRoofClick.BindUFunction(this, FName("RenderInteriorRoof"));
		InteriorUserWidgetHandler->OnInteriorWallClick.BindUFunction(this, FName("RenderInteriorWall"));

	}
	if (MaterialUserWidgetHandlerClass) {
		MaterialUserWidgetHandler = CreateWidget<UMaterialUserWidgetHandler>(this, MaterialUserWidgetHandlerClass);
		MaterialUserWidgetHandler->OnBuildingMaterialClick.BindUFunction(this, FName("ApplyBuildingMaterial"));
		MaterialUserWidgetHandler->OnRoadMaterialClick.BindUFunction(this, FName("ApplyRoadMaterial"));
	}

	UArchVizSavedSlotName* LoadSavedSlotNames = Cast<UArchVizSavedSlotName>(UGameplayStatics::LoadGameFromSlot(TEXT("SavedSlotNames"), 0));
	if (LoadSavedSlotNames)
	{
		for (FSlotNames SlotName : LoadSavedSlotNames->SlotNames) {
			SavedSlotNames.Add(SlotName.SlotName);
			CommonUserWidgetHandler->ComboBoxLoadSlotNames->AddOption(SlotName.SlotName);
		}
	}

}

void AArchVizController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

//Changing Modes

void AArchVizController::ChangeConstructionMode(FString Mode) {

	if (BuildingUserWidgetHandler->IsInViewport())
		BuildingUserWidgetHandler->RemoveFromParent();
	if (RoadUserWidgetHandler->IsInViewport())
		RoadUserWidgetHandler->RemoveFromParent();
	if (InteriorUserWidgetHandler->IsInViewport())
		InteriorUserWidgetHandler->RemoveFromParent();
	if (MaterialUserWidgetHandler->IsInViewport())
		MaterialUserWidgetHandler->RemoveFromParent();

	CleanUp();

	if (Mode == "Building") {
		BuildingUserWidgetHandler->AddToViewport();
		SetUpBuildingInputComponent();
		ConstructionMode = EConstructionMode::Building;
		BuildingUserWidgetHandler->BuildingConstructionMode->SetSelectedOption("Wall");
	}
	else if (Mode == "Road") {
		RoadUserWidgetHandler->AddToViewport();
		SetUpRoadInputComponent();
		ConstructionMode = EConstructionMode::Road;
		RoadBuilderNewRoadCreation();

	}
	else if (Mode == "Interior") {
		InteriorUserWidgetHandler->AddToViewport();
		SetUpInteriorInputComponent();
		ConstructionMode = EConstructionMode::Interior;
	}
	else if (Mode == "Material") {
		MaterialUserWidgetHandler->AddToViewport();
		SetUpMaterialInputComponent();
		ConstructionMode = EConstructionMode::Material;
	}
	else if (Mode == "View") {
		ClearAllModesMapping();
		ConstructionMode = EConstructionMode::View;
		CommonUserWidgetHandler->GuideButton->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AArchVizController::ClearAllModesMapping() {
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();
}

// CleanUps

void AArchVizController::CleanUp() {
	DestroyOverView();

	RoofLocations.Empty();
	FloorLocations.Empty();

	if (IsValid(SelectedWallMesh))
		SelectedWallMesh->SetRenderCustomDepth(false);

	if (bMoveWall) {
		if (IsValid(WallActor))
			WallActor->Destroy();

		bMoveWall = false;
	}

	if (bMoveInterior) {
		if (IsValid(InteriorActor))
			InteriorActor->Destroy();
		bMoveInterior = false;
	}

	if (bMoveInterior) {
		if (IsValid(InteriorActor))
			InteriorActor->Destroy();
	}

	if (IsValid(RoadActor))
		RoadActor->RoadProceduralMesh->SetRenderCustomDepth(false);

	if (IsValid(RoofFloor))
		RoofFloor->ProceduralMesh->SetRenderCustomDepth(false);

	DisableRendering(SelectedWallStaticMesh);

	if (IsValid(SelectedRoofActor))
		SelectedRoofActor->ProceduralMesh->SetRenderCustomDepth(false);
	if (IsValid(SelectedFloorActor))
		SelectedFloorActor->ProceduralMesh->SetRenderCustomDepth(false);

	CommonUserWidgetHandler->GuideButton->SetVisibility(ESlateVisibility::Visible);

}

//Road Construction

void AArchVizController::SetUpRoadInputComponent() {
	RoadBuilderMappingContext = NewObject<UInputMappingContext>(this);

	RoadBuilderMouseLeftClick = NewObject<UInputAction>(this);
	RoadBuilderMouseLeftClick->ValueType = EInputActionValueType::Boolean;

	RoadBuilderMouseRightClick = NewObject<UInputAction>(this);
	RoadBuilderMouseRightClick->ValueType = EInputActionValueType::Boolean;

	RoadBuilderDeleteKey = NewObject<UInputAction>(this);
	RoadBuilderDeleteKey->ValueType = EInputActionValueType::Boolean;

	RoadBuilderMiddleMouse = NewObject<UInputAction>(this);
	RoadBuilderMiddleMouse->ValueType = EInputActionValueType::Boolean;

	RoadBuilderMappingContext->MapKey(RoadBuilderMouseLeftClick, EKeys::LeftMouseButton);
	RoadBuilderMappingContext->MapKey(RoadBuilderMouseRightClick, EKeys::RightMouseButton);
	RoadBuilderMappingContext->MapKey(RoadBuilderDeleteKey, EKeys::Delete);
	RoadBuilderMappingContext->MapKey(RoadBuilderMiddleMouse, EKeys::MiddleMouseButton);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

	SetShowMouseCursor(true);

	check(EIC);

	EIC->BindAction(RoadBuilderMouseLeftClick, ETriggerEvent::Completed, this, &AArchVizController::RoadBuilderMouseLeftClickLocation);
	EIC->BindAction(RoadBuilderMouseRightClick, ETriggerEvent::Completed, this, &AArchVizController::RoadBuilderNewRoadCreation);
	EIC->BindAction(RoadBuilderDeleteKey, ETriggerEvent::Completed, this, &AArchVizController::DestroyRoad);
	EIC->BindAction(RoadBuilderMiddleMouse, ETriggerEvent::Completed, this, &AArchVizController::SelectRoadActor);

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(RoadBuilderMappingContext, 0);
}

void AArchVizController::RoadBuilderMouseLeftClickLocation() {

	RoadUserWidgetHandler->WidthUniformPanel->SetVisibility(ESlateVisibility::Hidden);
	RoadUserWidgetHandler->RoadMaterial->SetVisibility(ESlateVisibility::Hidden);

	if (IsValid(CurrentSelectedRoadActor))
		CurrentSelectedRoadActor->RoadProceduralMesh->SetRenderCustomDepth(false);


	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

	if (HitResult.bBlockingHit)
	{
		if (Cast<AWallActor>(HitResult.GetActor()) || Cast<ARoofAndFloorActor>(HitResult.GetActor()) || Cast<AInteriorActor>(HitResult.GetActor())) {
			SetInstructionText("Not Valid Point to place Road !!");
			return;
		}
		if (bFirstClick) {
			FirstClickLocation = HitResult.Location;
			bFirstClick = false;
		}
		else {
			SecondClickLocation = HitResult.Location;

			FVector MidPoint = (FirstClickLocation + SecondClickLocation) / 2;
			float RoadLength = FVector::Dist(FirstClickLocation, SecondClickLocation);

			FVector Direction = (SecondClickLocation - FirstClickLocation).GetSafeNormal();
			FVector NewDirection;
			FRotator Rotation;

			if (!bHasPreviousRoad) {

				NewDirection = Direction;
				//FirstClickLocation.X -= (Width * 1.3);
				//FirstClickLocation.Y -= (Width * 1.3);

				Rotation = UKismetMathLibrary::FindLookAtRotation(FirstClickLocation, SecondClickLocation);

				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				ARoadActor* RoadSpawnedActor = GetWorld()->SpawnActor<ARoadActor>(RoadActor->StaticClass(), MidPoint, Rotation, SpawnParams);

				if (RoadSpawnedActor) {
					RoadSpawnedActor->Initialize({ RoadLength, Width, Height });
					if (DefaultRoadMaterial)
						RoadSpawnedActor->ApplyRoadMaterial(DefaultRoadMaterial);
				}

				PreviousRoadDirection = NewDirection;
				PreviousRoadRotation = Rotation;
				PreviousClickLocation = SecondClickLocation;

				bHasPreviousRoad = true;
			}
			else {
				FVector RightVector = FVector::CrossProduct(PreviousRoadDirection, FVector::UpVector).GetSafeNormal();
				FVector LeftVector = -RightVector;

				FVector ClickDirection = (SecondClickLocation - PreviousClickLocation).GetSafeNormal();

				float DotStraight = FVector::DotProduct(ClickDirection, PreviousRoadDirection);
				float DotRight = FVector::DotProduct(ClickDirection, RightVector);
				float DotLeft = FVector::DotProduct(ClickDirection, LeftVector);

				if (DotRight > DotLeft && DotRight > DotStraight) {
					NewDirection = RightVector;
					Rotation = PreviousRoadRotation + FRotator(0, 90, 0);
				}
				else if (DotLeft > DotRight && DotLeft > DotStraight) {
					NewDirection = LeftVector;
					Rotation = PreviousRoadRotation + FRotator(0, -90, 0);
				}
				else {
					NewDirection = PreviousRoadDirection;
					Rotation = PreviousRoadRotation;
				}

				FVector NewEndPoint = PreviousClickLocation + (NewDirection * RoadLength);
				FVector NewMidPoint = (PreviousClickLocation + NewEndPoint) / 2;

				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				ARoadActor* RoadSpawnedActor = GetWorld()->SpawnActor<ARoadActor>(RoadActor->StaticClass(), NewMidPoint, Rotation, SpawnParams);

				if (RoadSpawnedActor) {
					RoadSpawnedActor->Initialize({ RoadLength, Width, Height });
					if (DefaultRoadMaterial)
						RoadSpawnedActor->ApplyRoadMaterial(DefaultRoadMaterial);
				}

				PreviousRoadDirection = NewDirection;
				PreviousRoadRotation = Rotation;
				PreviousClickLocation = NewEndPoint;
			}

			FirstClickLocation = SecondClickLocation;
			bFirstRoad = false;
		}
	}


}

void AArchVizController::RoadBuilderNewRoadCreation() {
	SetInstructionText("Constructing New Road...");
	bFirstClick = true;
	bFirstRoad = true;
	bHasPreviousRoad = false;
}

void AArchVizController::SelectRoadActor() {

	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

	if (IsValid(CurrentSelectedRoadActor))
		CurrentSelectedRoadActor->RoadProceduralMesh->SetRenderCustomDepth(false);

	if (HitResult.bBlockingHit)
	{
		ARoadActor* HitRoadActor = Cast<ARoadActor>(HitResult.GetActor());
		if (HitRoadActor) {
			CurrentSelectedRoadActor = HitRoadActor;

			RoadUserWidgetHandler->WidthUniformPanel->SetVisibility(ESlateVisibility::Visible);
			RoadUserWidgetHandler->RoadMaterial->SetVisibility(ESlateVisibility::Visible);

			CurrentSelectedRoadActor->RoadProceduralMesh->SetRenderCustomDepth(true);
			CurrentSelectedRoadActor->RoadProceduralMesh->SetCustomDepthStencilValue(2);

			RoadUserWidgetHandler->WidthSpinBox->SetValue(CurrentSelectedRoadActor->GetActorRelativeScale3D().Y * 300);
		}
		else {
			SetInstructionText("Not a Road !!");
		}
	}
}

void AArchVizController::ChangeRoadWidth(float width_) {
	if (CurrentSelectedRoadActor) {
		FVector Scale = CurrentSelectedRoadActor->GetActorRelativeScale3D();
		Scale.Y = width_ / 300;
		CurrentSelectedRoadActor->SetActorRelativeScale3D(Scale);
	}
}

void AArchVizController::ChangeRoadMaterial(const FRoadMaterial& RoadMaterial_)
{
	if (CurrentSelectedRoadActor) {
		CurrentSelectedRoadActor->ApplyRoadMaterial(RoadMaterial_.RoadMaterial);
		SetInstructionText("Road Material Changed.");
	}
}

void AArchVizController::DestroyRoad()
{
	if (CurrentSelectedRoadActor) {
		CurrentSelectedRoadActor->Destroy();
		SetInstructionText("Destroyed Road.");
	}
}


//Building Construction

void AArchVizController::SetUpBuildingInputComponent() {

	BuildingMappingContext = NewObject<UInputMappingContext>(this);

	BuildingMouseLeftClick = NewObject<UInputAction>(this);
	BuildingMouseLeftClick->ValueType = EInputActionValueType::Boolean;

	BuildingMouseRightClick = NewObject<UInputAction>(this);
	BuildingMouseRightClick->ValueType = EInputActionValueType::Boolean;

	BuildingRotateWall = NewObject<UInputAction>(this);
	BuildingRotateWall->ValueType = EInputActionValueType::Boolean;

	BuildingMiddleMouseButton = NewObject<UInputAction>(this);
	BuildingMiddleMouseButton->ValueType = EInputActionValueType::Boolean;

	BuildingDeleteWall = NewObject<UInputAction>(this);
	BuildingDeleteWall->ValueType = EInputActionValueType::Boolean;

	BuildingMappingContext->MapKey(BuildingMouseLeftClick, EKeys::LeftMouseButton);
	BuildingMappingContext->MapKey(BuildingMiddleMouseButton, EKeys::MiddleMouseButton);
	BuildingMappingContext->MapKey(BuildingMouseRightClick, EKeys::RightMouseButton);
	BuildingMappingContext->MapKey(BuildingRotateWall, EKeys::R);
	BuildingMappingContext->MapKey(BuildingDeleteWall, EKeys::Delete);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

	SetShowMouseCursor(true);

	check(EIC);

	EIC->BindAction(BuildingMouseLeftClick, ETriggerEvent::Completed, this, &AArchVizController::BuildingLeftMouseLogic);
	EIC->BindAction(BuildingMouseRightClick, ETriggerEvent::Completed, this, &AArchVizController::BuildingRightMouseLogic);
	EIC->BindAction(BuildingMiddleMouseButton, ETriggerEvent::Completed, this, &AArchVizController::BuildingMiddleMouseLogic);
	EIC->BindAction(BuildingRotateWall, ETriggerEvent::Completed, this, &AArchVizController::RotateWall);
	EIC->BindAction(BuildingDeleteWall, ETriggerEvent::Completed, this, &AArchVizController::DestroyWall);


	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(BuildingMappingContext, 0);

}

void AArchVizController::ChangeBuildingMode(FString Mode) {

	DisableRendering(SelectedWallStaticMesh);
	DisableRendering(SelectedInteriorStaticMesh);
	DestroyOverView();
	if (IsValid(SelectedRoofActor))
		SelectedRoofActor->ProceduralMesh->SetRenderCustomDepth(false);
	if (IsValid(SelectedFloorActor))
		SelectedFloorActor->ProceduralMesh->SetRenderCustomDepth(false);

	RoofLocations.Empty();
	FloorLocations.Empty();
	BuildingUserWidgetHandler->DoorTypes->SetVisibility(ESlateVisibility::Hidden);
	BuildingUserWidgetHandler->WindowTypes->SetVisibility(ESlateVisibility::Hidden);
	BuildingUserWidgetHandler->SegmentUniformPanel->SetVisibility(ESlateVisibility::Hidden);

	if (IsValid(SelectedWallMesh))
		SelectedWallMesh->SetRenderCustomDepth(false);

	if (Mode == "Wall") {
		BuildingMode = EBuildingMode::Wall;
		BuildingUserWidgetHandler->SegmentUniformPanel->SetVisibility(ESlateVisibility::Visible);
	}
	else if (Mode == "Roof") {
		BuildingMode = EBuildingMode::Roof;
		BuildingUserWidgetHandler->RoofFloorText->SetText(FText::FromString("Place Roof"));
	}
	else if (Mode == "Floor") {
		BuildingMode = EBuildingMode::Floor;
		BuildingUserWidgetHandler->RoofFloorText->SetText(FText::FromString("Place Floor"));
	}
	else if (Mode == "Door") {
		BuildingMode = EBuildingMode::Door;
	}
	else if (Mode == "Window") {
		BuildingMode = EBuildingMode::Window;
	}
	if (bMoveWall) {
		if (IsValid(WallActor))
			WallActor->Destroy();
		bMoveWall = false;
	}
}

void AArchVizController::BuildingLeftMouseLogic() {



	DisableRendering(SelectedWallStaticMesh);
	bMoveWall = false;

	if (BuildingMode == EBuildingMode::Wall)
		PlaceWall();
	else if (BuildingMode == EBuildingMode::Roof)
		StoreRoofPoints();
	else if (BuildingMode == EBuildingMode::Floor)
		StoreFloorPoints();
	else if (BuildingMode == EBuildingMode::Door)
		SelectWallSegment();
	else if (BuildingMode == EBuildingMode::Window)
		SelectWallSegment();

}

void AArchVizController::BuildingMiddleMouseLogic() {
	if (BuildingMode == EBuildingMode::Wall)
		SelectWallActor();
	else if (BuildingMode == EBuildingMode::Roof)
		SelectRoofActor();
	else if (BuildingMode == EBuildingMode::Floor)
		SelectFloorActor();
	else if (BuildingMode == EBuildingMode::Door)
		SelectDoor();
	else if (BuildingMode == EBuildingMode::Window)
		SelectWindow();
}

void AArchVizController::SelectWallActor()
{
	if (!bMoveWall) {
		FHitResult HitResult;
		FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true);

		FVector CursorWorldLocation;
		FVector CursorWorldDirection;
		DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

		GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams);

		if (AWallActor* HitWallActor = Cast<AWallActor>(HitResult.GetActor())) {
			bMoveWall = true;
			WallActor = HitWallActor;

			DisableRendering(SelectedWallStaticMesh);

			WallActor->GetComponents<UStaticMeshComponent>(SelectedWallStaticMesh);

			for (UStaticMeshComponent* Component : SelectedWallStaticMesh) {
				Component->SetRenderCustomDepth(true);
				Component->SetCustomDepthStencilValue(2);
			}

		}
		else {
			SetInstructionText("Not a Wall !!");
		}
	}
}

void AArchVizController::SelectFloorActor()
{
	FHitResult HitResult;
	FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true);

	FVector CursorWorldLocation;
	FVector CursorWorldDirection;
	DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

	GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams);

	if (ARoofAndFloorActor* HitFloorActor = Cast<ARoofAndFloorActor>(HitResult.GetActor())) {
		if (UProceduralMeshComponent* HitComponent = Cast<UProceduralMeshComponent>(HitResult.GetComponent())) {
			if (HitComponent->GetName().Contains("Floor"))
			{
				SelectedFloorActor = HitFloorActor;
				SelectedFloorActor->ProceduralMesh->SetRenderCustomDepth(true);
				SelectedFloorActor->ProceduralMesh->SetCustomDepthStencilValue(2);
			}

		}
	}
	else {
		SetInstructionText("Not a Floor !!");
	}
}

void AArchVizController::SelectRoofActor()
{
	FHitResult HitResult;
	FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true);

	FVector CursorWorldLocation;
	FVector CursorWorldDirection;
	DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

	GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams);

	if (ARoofAndFloorActor* HitRoofActor = Cast<ARoofAndFloorActor>(HitResult.GetActor())) {
		if (UProceduralMeshComponent* HitComponent = Cast<UProceduralMeshComponent>(HitResult.GetComponent())) {
			if (HitComponent->GetName().Contains("Roof"))
			{
				SelectedRoofActor = HitRoofActor;

				SelectedRoofActor->ProceduralMesh->SetRenderCustomDepth(true);
				SelectedRoofActor->ProceduralMesh->SetCustomDepthStencilValue(2);
			}

		}
	}
	else {
		SetInstructionText("Not a Roof !!");
	}
}

void AArchVizController::PlaceWall() {
	if (bMoveWall)
		bMoveWall = false;
}

void AArchVizController::DisableRendering(TArray<UStaticMeshComponent*> SelectedMesh)
{
	for (UStaticMeshComponent* Component : SelectedMesh) {
		if (Component)
			Component->SetRenderCustomDepth(false);
	}
	SelectedMesh.Empty();
}

void AArchVizController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (ConstructionMode == EConstructionMode::Building) {
		if (IsValid(WallActor) && bMoveWall) {
			FHitResult HitResult;
			FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, WallActor);

			FVector CursorWorldLocation;
			FVector CursorWorldDirection;
			DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

			if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams)) {
				FVector NewLocation = HitResult.Location;
				NewLocation.Z = 0;
				NewLocation.Z = BuildingUserWidgetHandler->CurrentStorey * 312;
				WallActor->SetActorRelativeLocation(NewLocation);
				SnapWall();
			}
		}
	}
	if (ConstructionMode == EConstructionMode::Interior) {
		if (IsValid(InteriorActor) && bMoveInterior) {
			FHitResult HitResult;
			FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, InteriorActor);

			FVector CursorWorldLocation;
			FVector CursorWorldDirection;
			DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

			if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams)) {
				FVector NewLocation = HitResult.Location;
				InteriorActor->SetActorRelativeLocation(NewLocation);

			}
		}
	}
}

void AArchVizController::SelectWallSegment() {
	if (!bMoveWall) {
		FHitResult HitResult;
		GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

		if (AWallActor* HitWallActor = Cast<AWallActor>(HitResult.GetActor())) {
			if (UStaticMeshComponent* HitComponent = Cast<UStaticMeshComponent>(HitResult.GetComponent())) {

				if (!HitComponent->GetName().Find("LowerWall")) {
					if (SelectedWallMesh) {
						SelectedWallMesh->SetRenderCustomDepth(false);
					}
					SelectedWallMesh = HitComponent;
					if (BuildingMode == EBuildingMode::Door)
						BuildingUserWidgetHandler->DoorTypes->SetVisibility(ESlateVisibility::Visible);
					else if (BuildingMode == EBuildingMode::Window)
						BuildingUserWidgetHandler->WindowTypes->SetVisibility(ESlateVisibility::Visible);
					SelectedWallMesh->SetRenderCustomDepth(true);
					SelectedWallMesh->CustomDepthStencilValue = 2.0;
					WallActor = HitWallActor;
				}

			}
		}

	}

}

void AArchVizController::SelectDoor() {
	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	if (SelectedDoorMesh) {
		SelectedDoorMesh->SetRenderCustomDepth(false);
	}
	if (AWallActor* HitWallActor = Cast<AWallActor>(HitResult.GetActor())) {
		if (UStaticMeshComponent* HitComponent = Cast<UStaticMeshComponent>(HitResult.GetComponent())) {

			if (!HitComponent->GetName().Find("Door")) {
				SelectedDoorMesh = HitComponent;
				SelectedDoorMesh->SetRenderCustomDepth(true);
				SelectedDoorMesh->CustomDepthStencilValue = 2.0;
				BuildingUserWidgetHandler->DoorTypes->SetVisibility(ESlateVisibility::Visible);
				WallActor = HitWallActor;

			}
		}
	}

}

void AArchVizController::SelectWindow() {
	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	if (SelectedWindowMesh) {
		SelectedWindowMesh->SetRenderCustomDepth(false);
	}
	if (AWallActor* HitWallActor = Cast<AWallActor>(HitResult.GetActor())) {
		if (UStaticMeshComponent* HitComponent = Cast<UStaticMeshComponent>(HitResult.GetComponent())) {

			if (!HitComponent->GetName().Find("Window")) {
				SelectedWindowMesh = HitComponent;
				SelectedWindowMesh->SetRenderCustomDepth(true);
				SelectedWindowMesh->CustomDepthStencilValue = 2.0;
				BuildingUserWidgetHandler->WindowTypes->SetVisibility(ESlateVisibility::Visible);
				WallActor = HitWallActor;
			}
		}
	}

}

void AArchVizController::BuildingRightMouseLogic() {
	if (BuildingMode == EBuildingMode::Wall) {
		NewWall();
	}
}

void AArchVizController::NewWall() {
	SetInstructionText("Constructing New Wall...");
	bMoveWall = true;
	WallActor = GetWorld()->SpawnActor<AWallActor>(AWallActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
	WallActor->GenerateSegmentedWalls(BuildingUserWidgetHandler->WallSegment->GetValue());
}

void AArchVizController::RotateWall() {
	if (IsValid(WallActor)) {
		FRotator NewRotation = WallActor->GetActorRotation();
		NewRotation.Yaw += 90;
		WallActor->SetActorRotation(NewRotation);
	}
}

void AArchVizController::SnapWall() {
	if (IsValid(WallActor)) {
		auto CurrentLocation = WallActor->GetActorLocation();

		CurrentLocation.X = FMath::RoundToFloat(CurrentLocation.X / WallSnapValue) * WallSnapValue;

		CurrentLocation.Y = FMath::RoundToFloat(CurrentLocation.Y / WallSnapValue) * WallSnapValue;

		WallActor->SetActorLocation(CurrentLocation);
	}
}

void AArchVizController::ChangeSegment(float Segments) {
	if (IsValid(WallActor) && bMoveWall) {
		WallActor->Destroy();
		WallActor = GetWorld()->SpawnActor<AWallActor>(AWallActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		WallActor->GenerateSegmentedWalls(Segments);
	}
}

void AArchVizController::PlaceDoor(const FDoorData& DoorData_) {
	if (SelectedWallMesh) {
		FTransform Transform = SelectedWallMesh->GetComponentTransform();

		WallActor->GenerateDoor(Transform, DoorData_.DoorMesh);

		BuildingUserWidgetHandler->DoorTypes->SetVisibility(ESlateVisibility::Hidden);

		SelectedWallMesh = nullptr;
	}
	if (SelectedDoorMesh) {
		FTransform Transform = SelectedDoorMesh->GetComponentTransform();

		FRotator CurrentRotation = Transform.GetRotation().Rotator();
		CurrentRotation.Yaw -= 90.0f;
		Transform.SetRotation(CurrentRotation.Quaternion());

		WallActor->GenerateDoor(Transform, DoorData_.DoorMesh);

		BuildingUserWidgetHandler->DoorTypes->SetVisibility(ESlateVisibility::Hidden);

		SelectedDoorMesh = nullptr;
	}
}

void AArchVizController::PlaceWindow(const FWindowData& WindowData_) {
	if (SelectedWallMesh) {
		FTransform Transform = SelectedWallMesh->GetComponentTransform();

		WallActor->GenerateWindow(Transform, WindowData_.WindowMesh, SelectedWallMesh->GetMaterial(0));

		BuildingUserWidgetHandler->WindowTypes->SetVisibility(ESlateVisibility::Hidden);

		SelectedWallMesh = nullptr;
	}
	if (SelectedWindowMesh) {
		FTransform Transform = SelectedWindowMesh->GetComponentTransform();

		WallActor->GenerateWindow(Transform, WindowData_.WindowMesh, SelectedWindowMesh->GetMaterial(0));

		BuildingUserWidgetHandler->WindowTypes->SetVisibility(ESlateVisibility::Hidden);

		SelectedWindowMesh = nullptr;
	}
}

void AArchVizController::StoreRoofOrFloorPoints(TArray<FVector>& Locations, bool bIsRoof)
{
	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

	if (AWallActor* HitWallActor = Cast<AWallActor>(HitResult.GetActor())) {
		if (UStaticMeshComponent* HitComponent = Cast<UStaticMeshComponent>(HitResult.GetComponent())) {
			if (!HitComponent->GetName().Find("UpperWall") || (!bIsRoof && !HitComponent->GetName().Find("LowerWall"))) {
				FVector WallTopLeft = HitComponent->GetSocketLocation("WallTopLeftSocket");
				FVector WallTopRight = HitComponent->GetSocketLocation("WallTopRightSocket");
				FVector HitLocation = HitResult.Location;
				FVector ClosestPoint = (FVector::Dist(WallTopLeft, HitLocation) < FVector::Dist(WallTopRight, HitLocation)) ? WallTopLeft : WallTopRight;
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Black, FString::FromInt(ClosestPoint.Z));

				if (!bIsRoof) ClosestPoint.Z = BuildingUserWidgetHandler->CurrentStorey * 310;
				//if (!bIsRoof) ClosestPoint.Z = 1;
				else
				{
					ClosestPoint.Z -= 5;
				}

				if (Locations.Num() < 4) {
					Locations.Add(ClosestPoint);
				}
				else {
					Locations.Empty();
					DestroyOverView();
					Locations.Add(ClosestPoint);
				}
			}
		}
	}
	else if (!bIsRoof) {
		if (Locations.Num() < 4) {
			//HitResult.Location.Z = 1;
			HitResult.Location.Z = BuildingUserWidgetHandler->CurrentStorey * 310;
			Locations.Add(HitResult.Location);
		}
		else {
			Locations.Empty();
			DestroyOverView();
			//HitResult.Location.Z = 1;
			HitResult.Location.Z = BuildingUserWidgetHandler->CurrentStorey * 310;
			Locations.Add(HitResult.Location);
		}
	}

	if (Locations.Num() > 1) {
		DrawTemporaryLine(Locations[Locations.Num() - 2], Locations[Locations.Num() - 1], bIsRoof);
	}
	FString text = "Point " + FString::FromInt(Locations.Num()) + " Stored";
	SetInstructionText(text, 1);
	if (Locations.Num() == 4) {
		BuildingUserWidgetHandler->RoofFloorButton->SetVisibility(ESlateVisibility::Visible);
		DrawTemporaryLine(Locations[Locations.Num() - 1], Locations[0], bIsRoof);
	}
}

void AArchVizController::DrawTemporaryLine(FVector Start, FVector End, bool bIsRoof)
{
	FActorSpawnParameters SpawnParams;
	ARoofAndFloorActor* OverViewLine = GetWorld()->SpawnActor<ARoofAndFloorActor>(ARoofAndFloorActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	if (OverViewLine) {

		OverViewLine->GenerateOverviewMesh(Start, End, 20.0f, 20.0f);
		OverviewActors.Add(OverViewLine);
		OverViewLine->LineMesh->SetRenderCustomDepth(true);
		OverViewLine->LineMesh->CustomDepthStencilValue = 2.0;
	}
}

void AArchVizController::StoreRoofPoints() {
	StoreRoofOrFloorPoints(RoofLocations, true);
}

void AArchVizController::StoreFloorPoints() {
	StoreRoofOrFloorPoints(FloorLocations, false);
}

void AArchVizController::PlaceRoofFloor() {
	TArray<FVector>& Locations = (BuildingMode == EBuildingMode::Roof) ? RoofLocations : FloorLocations;
	bool bIsRoof = (BuildingMode == EBuildingMode::Roof);

	FActorSpawnParameters SpawnParams;
	ARoofAndFloorActor* RoofOrFloorActor = GetWorld()->SpawnActor<ARoofAndFloorActor>(ARoofAndFloorActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	if (RoofOrFloorActor) {
		bIsRoof ? RoofOrFloorActor->GenerateRoof(Locations, BuildingUserWidgetHandler->CurrentStorey) : RoofOrFloorActor->GenerateFloor(Locations, BuildingUserWidgetHandler->CurrentStorey);
		RoofOrFloorActor->ApplyMaterial(WallMaterial);
	}

	DestroyOverView();
	Locations.Empty();
}

void AArchVizController::DestroyOverView() {
	for (int i = 0; i < OverviewActors.Num(); ++i) {
		if (OverviewActors[i])
			OverviewActors[i]->Destroy();
	}
	BuildingUserWidgetHandler->RoofFloorButton->SetVisibility(ESlateVisibility::Hidden);
}

void AArchVizController::DestroyWall()
{
	if (BuildingMode == EBuildingMode::Wall)
	{
		if (IsValid(WallActor)) {
			WallActor->Destroy();
			bMoveWall = false;
			SetInstructionText("Wall Destroyed.");
		}
	}
	else if (BuildingMode == EBuildingMode::Roof)
	{
		if (IsValid(SelectedRoofActor)) {
			SelectedRoofActor->Destroy();
			SetInstructionText("Roof Destroyed.");
		}
	}
	else if (BuildingMode == EBuildingMode::Floor)
	{
		if (IsValid(SelectedFloorActor)) {
			SelectedFloorActor->Destroy();
			SetInstructionText("Floor Destroyed.");
		}
	}

}

void AArchVizController::IncrementStoreyCheck()
{
	TArray<AActor*> FoundRoofFloorActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARoofAndFloorActor::StaticClass(), FoundRoofFloorActors);

	for (AActor* Actor : FoundRoofFloorActors)
	{
		ARoofAndFloorActor* RoofFloorActor_ = Cast<ARoofAndFloorActor>(Actor);
		if (RoofFloorActor_)
		{
			if (RoofFloorActor_->RoofAtStorey == BuildingUserWidgetHandler->CurrentStorey)
			{
				++BuildingUserWidgetHandler->CurrentStorey;
				return;
			}
		}
	}
	SetInstructionText("First Add Roof For Current Storey !!");

}

//Interior Design 

void AArchVizController::SetUpInteriorInputComponent() {

	InteriorMappingContext = NewObject<UInputMappingContext>(this);

	InteriorMouseLeftClick = NewObject<UInputAction>(this);
	InteriorMouseLeftClick->ValueType = EInputActionValueType::Boolean;

	InteriorMouseMiddleClick = NewObject<UInputAction>(this);
	InteriorMouseMiddleClick->ValueType = EInputActionValueType::Boolean;

	InteriorDelete = NewObject<UInputAction>(this);
	InteriorDelete->ValueType = EInputActionValueType::Boolean;

	InteriorRotate = NewObject<UInputAction>(this);
	InteriorRotate->ValueType = EInputActionValueType::Boolean;

	InteriorMappingContext->MapKey(InteriorMouseLeftClick, EKeys::LeftMouseButton);
	InteriorMappingContext->MapKey(InteriorMouseMiddleClick, EKeys::MiddleMouseButton);
	InteriorMappingContext->MapKey(InteriorRotate, EKeys::R);
	InteriorMappingContext->MapKey(InteriorDelete, EKeys::Delete);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

	SetShowMouseCursor(true);

	check(EIC);

	EIC->BindAction(InteriorMouseLeftClick, ETriggerEvent::Completed, this, &AArchVizController::InteriorLeftMouseLogic);
	EIC->BindAction(InteriorMouseMiddleClick, ETriggerEvent::Completed, this, &AArchVizController::SelectInteriorActor);
	EIC->BindAction(InteriorRotate, ETriggerEvent::Completed, this, &AArchVizController::RotateInterior);
	EIC->BindAction(InteriorDelete, ETriggerEvent::Completed, this, &AArchVizController::DestroyInterior);

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InteriorMappingContext, 0);

}

void AArchVizController::InteriorLeftMouseLogic()
{
	if (bMoveInterior) {
		FHitResult HitResult;
		FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, InteriorActor);

		FVector CursorWorldLocation;
		FVector CursorWorldDirection;
		DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

		GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams);


		if (InteriorType == EInteriorType::InteriorWall) {
			if (AWallActor* HitWallActor = Cast<AWallActor>(HitResult.GetActor())) {
				//if (!CheckInteriorCollision(HitResult.Location, GetActorExtentFromMesh(InteriorActor->InteriorActorMesh), InteriorActor)) {
				bMoveInterior = false;
				DisableRendering(SelectedInteriorStaticMesh);
				//}
			}
		}
		else {
			if (ARoofAndFloorActor* RoofFloorActor = Cast<ARoofAndFloorActor>(HitResult.GetActor())) {
				if (UProceduralMeshComponent* HitComponent = Cast<UProceduralMeshComponent>(HitResult.GetComponent())) {
					if ((HitComponent->GetName().Contains("Roof") && InteriorType == EInteriorType::InteriorRoof) || (HitComponent->GetName().Contains("Floor") && InteriorType == EInteriorType::InteriorFloor)) {
						//if (!CheckInteriorCollision(HitResult.Location, GetActorExtentFromMesh(InteriorActor->InteriorActorMesh), InteriorActor)) {
						bMoveInterior = false;
						DisableRendering(SelectedInteriorStaticMesh);
						//}
					}
				}
			}
		}
		if (bMoveInterior)
			SetInstructionText("Not Valid Location to place Interior");

	}


}

bool AArchVizController::CheckInteriorCollision(const FVector& Location, const FVector& ActorExtent, AInteriorActor* InteriorActorIgnore)
{
	FCollisionShape CollisionShape = FCollisionShape::MakeBox(ActorExtent);
	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	if (InteriorActorIgnore) {

		CollisionParams.AddIgnoredActor(InteriorActorIgnore);

	}
	bool bHasCollision = GetWorld()->OverlapMultiByChannel(
		Overlaps,
		Location,
		FQuat::Identity,
		ECollisionChannel::ECC_Visibility,
		CollisionShape,
		CollisionParams
	);

	if (bHasCollision) {
		for (const FOverlapResult& Overlap : Overlaps) {
			AActor* OverlappingActor = Overlap.GetActor();
			if (OverlappingActor) {
				AInteriorActor* CustomInteriorActor = Cast<AInteriorActor>(OverlappingActor);
				if (CustomInteriorActor) {
					GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Collision detected with another interior actor."));
					return true;
				}
			}
		}
	}
	return false;
}

FVector AArchVizController::GetActorExtentFromMesh(UStaticMesh* Mesh) {
	if (Mesh) {
		FVector Origin, BoxExtent;
		Mesh->GetBounds().GetBox().GetCenterAndExtents(Origin, BoxExtent);
		return BoxExtent;
	}
	return FVector(50.0f, 50.0f, 50.0f);
}

void AArchVizController::SelectInteriorActor()
{
	if (!bMoveInterior) {
		FHitResult HitResult;
		FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true);

		FVector CursorWorldLocation;
		FVector CursorWorldDirection;
		DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

		GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams);

		if (AInteriorActor* HitInteriorActor = Cast<AInteriorActor>(HitResult.GetActor())) {

			if (UStaticMeshComponent* HitComponent = Cast<UStaticMeshComponent>(HitResult.GetComponent())) {
				if (!HitComponent->GetName().Find("Floor")) {
					InteriorType = EInteriorType::InteriorFloor;
				}
				else if (!HitComponent->GetName().Find("Roof")) {
					InteriorType = EInteriorType::InteriorRoof;
				}
				else if (!HitComponent->GetName().Find("Wall")) {
					InteriorType = EInteriorType::InteriorWall;
				}
				bMoveInterior = true;
				InteriorActor = HitInteriorActor;
				InteriorActor->GetComponents<UStaticMeshComponent>(SelectedInteriorStaticMesh);

				SelectedInteriorStaticMesh[0]->SetRenderCustomDepth(true);
				SelectedInteriorStaticMesh[0]->SetCustomDepthStencilValue(2);

			}

		}
		else {
			SetInstructionText("Not a Interior!!");
		}
	}
}

void AArchVizController::RotateInterior() {
	if (IsValid(InteriorActor)) {
		FRotator NewRotation = InteriorActor->GetActorRotation();
		NewRotation.Yaw += 20;
		InteriorActor->SetActorRotation(NewRotation);
	}
}

void AArchVizController::RenderInteriorFloor(const FInteriorFloorData& FloorData_) {

	if (IsValid(InteriorActor) && bMoveInterior)
		InteriorActor->Destroy();
	InteriorType = EInteriorType::InteriorFloor;
	bMoveInterior = true;
	InteriorActor = GetWorld()->SpawnActor<AInteriorActor>(AInteriorActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
	InteriorActor->GenerateInterior(FloorData_.InteriorFloorMesh, "Floor");
	InteriorActor->GetComponents<UStaticMeshComponent>(SelectedInteriorStaticMesh);

	SelectedInteriorStaticMesh[0]->SetRenderCustomDepth(true);
	SelectedInteriorStaticMesh[0]->SetCustomDepthStencilValue(2);
}

void AArchVizController::RenderInteriorRoof(const FInteriorRoofData& RoofData_) {

	if (IsValid(InteriorActor) && bMoveInterior)
		InteriorActor->Destroy();
	InteriorType = EInteriorType::InteriorRoof;
	bMoveInterior = true;
	InteriorActor = GetWorld()->SpawnActor<AInteriorActor>(AInteriorActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
	InteriorActor->GenerateInterior(RoofData_.InteriorRoofMesh, "Roof");
	InteriorActor->GetComponents<UStaticMeshComponent>(SelectedInteriorStaticMesh);

	SelectedInteriorStaticMesh[0]->SetRenderCustomDepth(true);
	SelectedInteriorStaticMesh[0]->SetCustomDepthStencilValue(2);
}

void AArchVizController::RenderInteriorWall(const FInteriorWallData& WallData_) {

	if (IsValid(InteriorActor) && bMoveInterior)
		InteriorActor->Destroy();
	InteriorType = EInteriorType::InteriorWall;
	bMoveInterior = true;
	InteriorActor = GetWorld()->SpawnActor<AInteriorActor>(AInteriorActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
	InteriorActor->GenerateInterior(WallData_.InteriorWallMesh, "Wall");
	InteriorActor->GetComponents<UStaticMeshComponent>(SelectedInteriorStaticMesh);

	SelectedInteriorStaticMesh[0]->SetRenderCustomDepth(true);
	SelectedInteriorStaticMesh[0]->SetCustomDepthStencilValue(2);

}

void AArchVizController::DestroyInterior()
{
	if (IsValid(InteriorActor)) {
		InteriorActor->Destroy();
		bMoveInterior = false;
		SetInstructionText("Interior Destroyed.");
	}
}

//Material Design 

void AArchVizController::SetUpMaterialInputComponent() {

	MaterialMappingContext = NewObject<UInputMappingContext>(this);

	MaterialMouseLeftClick = NewObject<UInputAction>(this);
	MaterialMouseLeftClick->ValueType = EInputActionValueType::Boolean;

	MaterialMappingContext->MapKey(MaterialMouseLeftClick, EKeys::LeftMouseButton);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

	SetShowMouseCursor(true);

	check(EIC);

	EIC->BindAction(MaterialMouseLeftClick, ETriggerEvent::Completed, this, &AArchVizController::MaterialLeftMouseLogic);

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(MaterialMappingContext, 0);

}

void AArchVizController::MaterialLeftMouseLogic()
{
	if (IsValid(RoadActor))
		RoadActor->RoadProceduralMesh->SetRenderCustomDepth(false);
	if (IsValid(RoofFloor))
		RoofFloor->ProceduralMesh->SetRenderCustomDepth(false);
	DisableRendering(SelectedWallStaticMesh);

	FHitResult HitResult;
	FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true);

	FVector CursorWorldLocation;
	FVector CursorWorldDirection;
	DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

	GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams);

	if (Cast<ARoofAndFloorActor>(HitResult.GetActor()) || Cast<AWallActor>(HitResult.GetActor())) {
		if (Cast<ARoofAndFloorActor>(HitResult.GetActor())) {
			MaterialType = EMaterialType::RoofFloorMaterial;
			RoofFloor = Cast<ARoofAndFloorActor>(HitResult.GetActor());
			RoofFloor->ProceduralMesh->SetRenderCustomDepth(true);
			RoofFloor->ProceduralMesh->SetCustomDepthStencilValue(2);
		}
		else {
			MaterialType = EMaterialType::WallMaterial;
			WallActor = Cast<AWallActor>(HitResult.GetActor());
			WallActor->GetComponents<UStaticMeshComponent>(SelectedWallStaticMesh);

			for (UStaticMeshComponent* Component : SelectedWallStaticMesh) {
				Component->SetRenderCustomDepth(true);
				Component->SetCustomDepthStencilValue(2);
			}
		}

		MaterialUserWidgetHandler->BuildingMaterialContainer->SetVisibility(ESlateVisibility::Visible);
		MaterialUserWidgetHandler->RoadMaterialContainer->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (Cast<ARoadActor>(HitResult.GetActor())) {
		MaterialType = EMaterialType::RoadMaterial;
		RoadActor = Cast<ARoadActor>(HitResult.GetActor());
		MaterialUserWidgetHandler->BuildingMaterialContainer->SetVisibility(ESlateVisibility::Hidden);
		MaterialUserWidgetHandler->RoadMaterialContainer->SetVisibility(ESlateVisibility::Visible);
		RoadActor->RoadProceduralMesh->SetRenderCustomDepth(true);
		RoadActor->RoadProceduralMesh->SetCustomDepthStencilValue(2);
	}

}

void AArchVizController::ApplyBuildingMaterial(const FBuildingMaterial& BuildingMaterial) {
	if (MaterialType == EMaterialType::WallMaterial) {
		if (IsValid(WallActor))
			WallActor->ApplyMaterial(BuildingMaterial.BuildingMaterial);
	}
	else if (MaterialType == EMaterialType::RoofFloorMaterial) {
		if (IsValid(RoofFloor))
			RoofFloor->ApplyMaterial(BuildingMaterial.BuildingMaterial);
	}
}

void AArchVizController::ApplyRoadMaterial(const FRoadMaterial& RoadMaterial) {
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Emerald, "Road");
	if (MaterialType == EMaterialType::RoadMaterial) {
		if (IsValid(RoadActor))
			RoadActor->ApplyRoadMaterial(RoadMaterial.RoadMaterial);
	}
}

//Save And Load

void AArchVizController::SaveArchviz()
{
	FString SlotName = CommonUserWidgetHandler->FileNameText->GetText().ToString();

	UArchVizSaveGame* SaveArchVizInstance = Cast<UArchVizSaveGame>(UGameplayStatics::CreateSaveGameObject(UArchVizSaveGame::StaticClass()));

	TArray<AActor*> FoundRoadActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARoadActor::StaticClass(), FoundRoadActors);

	for (AActor* Actor : FoundRoadActors)
	{
		ARoadActor* RoadActor_ = Cast<ARoadActor>(Actor);
		if (RoadActor_)
		{
			FRoadActorData RoadData;
			RoadData.RoadTransform = RoadActor_->GetActorTransform();
			RoadData.RoadMaterial = Cast<UMaterialInstance>(RoadActor_->RoadProceduralMesh->GetMaterial(0));
			RoadData.RoadDimentions = RoadActor_->RoadDimention;

			SaveArchVizInstance->RoadActorArray.Add(RoadData);
		}
	}

	TArray<AActor*> FoundWallActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWallActor::StaticClass(), FoundWallActors);

	for (AActor* Actor : FoundWallActors)
	{
		AWallActor* WallActor_ = Cast<AWallActor>(Actor);
		if (WallActor_)
		{
			FWallActorData WallData;
			WallData.WallTransform = WallActor_->GetActorTransform();
			WallData.WallMaterial = Cast<UMaterialInstance>(WallActor_->WallMaterial);
			WallData.NumberofSegments = WallActor_->NumberofSegments;

			for (int i = 0; i < WallActor_->DoorStaticMeshArray.Num(); ++i) {
				WallData.DoorTransformArray.Add(WallActor_->DoorTransformArray[i]);
				WallData.DoorStaticMeshArray.Add(WallActor_->DoorStaticMeshArray[i]);
			}

			for (int i = 0; i < WallActor_->WindowStaticMeshArray.Num(); ++i) {
				WallData.WindowTransformArray.Add(WallActor_->WindowTransformArray[i]);
				WallData.WindowStaticMeshArray.Add(WallActor_->WindowStaticMeshArray[i]);
			}

			SaveArchVizInstance->WallActorArray.Add(WallData);
		}
	}

	TArray<AActor*> FoundRoofFloorActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARoofAndFloorActor::StaticClass(), FoundRoofFloorActors);

	for (AActor* Actor : FoundRoofFloorActors)
	{
		ARoofAndFloorActor* RoofFloorActor_ = Cast<ARoofAndFloorActor>(Actor);
		if (RoofFloorActor_)
		{
			FRoofFloorActorData RoofFloorData;
			RoofFloorData.RoofFloorTransform = RoofFloorActor_->GetActorTransform();
			RoofFloorData.RoofFloorMaterial = Cast<UMaterialInstance>(RoofFloorActor_->ProceduralMesh->GetMaterial(0));
			RoofFloorData.ActorType = RoofFloorActor_->ActorType;
			RoofFloorData.RoofAtStorey = RoofFloorActor_->RoofAtStorey;

			for (int i = 0; i < RoofFloorActor_->RoofFloorVertices.Num(); ++i) {
				RoofFloorData.RoofFloorVertices.Add(RoofFloorActor_->RoofFloorVertices[i]);
			}

			SaveArchVizInstance->RoofFloorActorArray.Add(RoofFloorData);
		}
	}

	TArray<AActor*> FoundInteriorActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteriorActor::StaticClass(), FoundInteriorActors);

	for (AActor* Actor : FoundInteriorActors)
	{
		AInteriorActor* InteriorActor_ = Cast<AInteriorActor>(Actor);
		if (InteriorActor_)
		{
			FInteriorActorData InteriorData;
			InteriorData.InteriorTransform = InteriorActor_->GetActorTransform();
			InteriorData.InteriorActorType = InteriorActor_->InteriorActorType;
			InteriorData.InteriorMesh = InteriorActor_->InteriorActorMesh;

			SaveArchVizInstance->InteriorActorArray.Add(InteriorData);
		}
	}

	if (UGameplayStatics::SaveGameToSlot(SaveArchVizInstance, SlotName, 0))
	{
		UArchVizSavedSlotName* SaveSlotNameInstance = Cast<UArchVizSavedSlotName>(UGameplayStatics::CreateSaveGameObject(UArchVizSavedSlotName::StaticClass()));

		UArchVizSavedSlotName* LoadSavedSlotNames = Cast<UArchVizSavedSlotName>(UGameplayStatics::LoadGameFromSlot(TEXT("SavedSlotNames"), 0));
		if (LoadSavedSlotNames)
		{
			SaveSlotNameInstance->SlotNames = LoadSavedSlotNames->SlotNames;
		}

		FSlotNames SlotNameStruct;
		SlotNameStruct.SlotName = SlotName;
		SaveSlotNameInstance->SlotNames.Add(SlotNameStruct);

		if (UGameplayStatics::SaveGameToSlot(SaveSlotNameInstance, "SavedSlotNames", 0))
		{
			SetInstructionText("ArchViz Saved: " + SlotName);
		}
	}

}

void AArchVizController::LoadArchviz(FString SlotNameToLoad)
{
	UArchVizSaveGame* LoadArchVizInstance = Cast<UArchVizSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotNameToLoad, 0));
	if (!LoadArchVizInstance) return;

	for (const FRoadActorData& RoadData : LoadArchVizInstance->RoadActorArray)
	{
		FActorSpawnParameters SpawnParams;
		ARoadActor* RoadActor_ = GetWorld()->SpawnActor<ARoadActor>(ARoadActor::StaticClass(), RoadData.RoadTransform, SpawnParams);
		if (RoadActor_)
		{
			RoadActor_->Initialize(RoadData.RoadDimentions);
			RoadActor_->ApplyRoadMaterial(RoadData.RoadMaterial);
		}
	}

	for (const FWallActorData& WallData : LoadArchVizInstance->WallActorArray)
	{
		FActorSpawnParameters SpawnParams;
		AWallActor* WallActor_ = GetWorld()->SpawnActor<AWallActor>(AWallActor::StaticClass(), WallData.WallTransform, SpawnParams);
		if (WallActor_)
		{
			WallActor_->GenerateSegmentedWalls(WallData.NumberofSegments);
			WallActor_->ApplyMaterial(WallData.WallMaterial);

			for (int i = 0; i < WallData.DoorTransformArray.Num(); ++i)
			{
				WallActor_->GenerateDoor(WallData.DoorTransformArray[i], WallData.DoorStaticMeshArray[i]);
			}

			for (int i = 0; i < WallData.WindowTransformArray.Num(); ++i)
			{
				WallActor_->GenerateWindow(WallData.WindowTransformArray[i], WallData.WindowStaticMeshArray[i], WallData.WallMaterial);
			}

		}
	}

	for (const FRoofFloorActorData& RoofFloorData : LoadArchVizInstance->RoofFloorActorArray)
	{
		FActorSpawnParameters SpawnParams;
		ARoofAndFloorActor* RoofFloorActor_ = GetWorld()->SpawnActor<ARoofAndFloorActor>(ARoofAndFloorActor::StaticClass(), RoofFloorData.RoofFloorTransform, SpawnParams);
		if (RoofFloorActor_)
		{
			RoofFloorActor_->GenerateMesh(RoofFloorData.RoofFloorVertices, RoofFloorData.ActorType);
			RoofFloorActor_->RoofAtStorey = RoofFloorData.RoofAtStorey;
			RoofFloorActor_->ApplyMaterial(RoofFloorData.RoofFloorMaterial);
		}
	}

	for (const FInteriorActorData& InteriorData : LoadArchVizInstance->InteriorActorArray)
	{
		FActorSpawnParameters SpawnParams;
		AInteriorActor* InteriorActor_ = GetWorld()->SpawnActor<AInteriorActor>(AInteriorActor::StaticClass(), InteriorData.InteriorTransform, SpawnParams);
		if (InteriorActor_)
		{
			InteriorActor_->GenerateInterior(InteriorData.InteriorMesh, InteriorData.InteriorActorType);
		}
	}
	CommonUserWidgetHandler->ComboBoxLoadSlotNames->SetVisibility(ESlateVisibility::Hidden);

}

void AArchVizController::LoadArchvizSlotNames() {
	CommonUserWidgetHandler->ComboBoxLoadSlotNames->SetVisibility(ESlateVisibility::Visible);
}
//
void AArchVizController::HideInstructionText() {
	CommonUserWidgetHandler->InstructionSwitcher->SetVisibility(ESlateVisibility::Hidden);
	CommonUserWidgetHandler->InstructionText->SetVisibility(ESlateVisibility::Hidden);
}

void AArchVizController::ShowModeSpecificInstruction() {
	if (ConstructionMode == EConstructionMode::Building) {
		if (BuildingMode == EBuildingMode::Wall) {
			CommonUserWidgetHandler->InstructionSwitcher->SetActiveWidgetIndex(1);
		}
		else if (BuildingMode == EBuildingMode::Roof) {
			CommonUserWidgetHandler->InstructionSwitcher->SetActiveWidgetIndex(2);
		}
		else if (BuildingMode == EBuildingMode::Floor) {
			CommonUserWidgetHandler->InstructionSwitcher->SetActiveWidgetIndex(3);
		}
		else if (BuildingMode == EBuildingMode::Door) {
			CommonUserWidgetHandler->InstructionSwitcher->SetActiveWidgetIndex(4);
		}
		else if (BuildingMode == EBuildingMode::Window) {
			CommonUserWidgetHandler->InstructionSwitcher->SetActiveWidgetIndex(5);
		}
	}
	else if (ConstructionMode == EConstructionMode::Road) {
		CommonUserWidgetHandler->InstructionSwitcher->SetActiveWidgetIndex(0);
	}
	else if (ConstructionMode == EConstructionMode::Interior) {
		CommonUserWidgetHandler->InstructionSwitcher->SetActiveWidgetIndex(6);
	}
	else if (ConstructionMode == EConstructionMode::Material) {
		CommonUserWidgetHandler->InstructionSwitcher->SetActiveWidgetIndex(7);
	}
	CommonUserWidgetHandler->InstructionSwitcher->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(Timehandle, this, &AArchVizController::HideInstructionText, 3, false);

}

void AArchVizController::SetInstructionText(FString Text, int time) {
	CommonUserWidgetHandler->InstructionText->SetVisibility(ESlateVisibility::Visible);
	CommonUserWidgetHandler->InstructionText->SetText(FText::FromString(Text));
	GetWorld()->GetTimerManager().SetTimer(Timehandle, this, &AArchVizController::HideInstructionText, time, false);
}