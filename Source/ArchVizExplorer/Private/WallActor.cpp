// Fill out your copyright notice in the Description page of Project Settings.


#include "WallActor.h"

// Sets default values
AWallActor::AWallActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	RootComponent = RootScene;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WallMeshFinder(TEXT("/Script/Engine.StaticMesh'/Game/ArchVizExplorerContent/StaticMeshes/CustomWall.CustomWall'"));
	if (WallMeshFinder.Succeeded())
	{
		WallMesh = WallMeshFinder.Object;
	}

}

void AWallActor::GenerateSegmentedWalls(int Segments) {

	DestroyPreviousWalls();
	NumberofSegments = Segments;

	for (int i = 0; i < Segments; ++i)
	{
		FString LowerWallName = "LowerWall" + FString::FromInt(i);
		UStaticMeshComponent* LowerWall = NewObject<UStaticMeshComponent>(this, *LowerWallName);
		LowerWall->AttachToComponent(RootScene, FAttachmentTransformRules::KeepRelativeTransform);
		LowerWall->SetRelativeLocation(FVector((i * 114), 0, 0));

		LowerWall->RegisterComponentWithWorld(GetWorld());

		FString UpperWallName = "UpperWall" + FString::FromInt(i);
		UStaticMeshComponent* UpperWall = NewObject<UStaticMeshComponent>(this, *UpperWallName);
		UpperWall->AttachToComponent(RootScene, FAttachmentTransformRules::KeepRelativeTransform);
		UpperWall->SetRelativeLocation(FVector(LowerWall->GetRelativeLocation().X, LowerWall->GetRelativeLocation().Y, LowerWall->GetRelativeLocation().Z + 212));

		UpperWall->RegisterComponentWithWorld(GetWorld());

		LowerWall->SetStaticMesh(WallMesh);
		LowerWall->SetRelativeScale3D(FVector(0.285f, 1.0f, 1.06f));

		UpperWall->SetStaticMesh(WallMesh);
		UpperWall->SetRelativeScale3D(FVector(0.285f, 1.0f, 0.5f));

		LowerWallArray.Add(LowerWall);
		UpperWallArray.Add(UpperWall);

	}
}

void AWallActor::DestroyComponentByTransform(const FTransform& Transform)
{
	TArray<USceneComponent*> AttachedComponents;
	RootScene->GetChildrenComponents(true, AttachedComponents);

	for (USceneComponent* Component : AttachedComponents)
	{
		if (Component && Component->GetComponentTransform().Equals(Transform, 1.0f))
		{
			Component->DestroyComponent();
			return;
		}
	}
}

void AWallActor::GenerateDoor(FTransform DoorTransform, UStaticMesh* DoorMesh)
{
	if (DoorMesh)
	{
		DestroyComponentByTransform(DoorTransform);
		DoorTransformArray.Add(DoorTransform);
		DoorStaticMeshArray.Add(DoorMesh);

		FString DoorName = "Door" + FString::FromInt(FMath::RandRange(0, 1000000));
		UStaticMeshComponent* Door = NewObject<UStaticMeshComponent>(this, *DoorName);
		Door->AttachToComponent(RootScene, FAttachmentTransformRules::KeepRelativeTransform);

		FRotator CurrentRotation = DoorTransform.GetRotation().Rotator();
		CurrentRotation.Yaw += 90.0f;
		DoorTransform.SetRotation(CurrentRotation.Quaternion());

		DoorTransform.SetScale3D({ 1,1,1 });

		Door->SetWorldTransform(DoorTransform);
		Door->SetWorldTransform(DoorTransform);
		Door->SetStaticMesh(DoorMesh);
		Door->RegisterComponentWithWorld(GetWorld());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DoorMesh is not assigned."));
	}
}

void AWallActor::GenerateWindow(FTransform WindowTransform, UStaticMesh* WindowMesh, UMaterialInterface* Material)
{
	if (WindowMesh)
	{
		DestroyComponentByTransform(WindowTransform);
		WindowTransformArray.Add(WindowTransform);
		WindowStaticMeshArray.Add(WindowMesh);
		FString WindowName = "Window" + FString::FromInt(FMath::RandRange(0, 1000000));

		UStaticMeshComponent* Window = NewObject<UStaticMeshComponent>(this, *WindowName);
		Window->AttachToComponent(RootScene, FAttachmentTransformRules::KeepRelativeTransform);

		FRotator CurrentRotation = WindowTransform.GetRotation().Rotator();
		WindowTransform.SetRotation(CurrentRotation.Quaternion());

		WindowTransform.SetScale3D({ 1,1,1 });

		Window->SetWorldTransform(WindowTransform);
		Window->SetWorldTransform(WindowTransform);
		Window->SetStaticMesh(WindowMesh);
		Window->RegisterComponentWithWorld(GetWorld());
		Window->SetMaterial(1, Material);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WindowMesh is not assigned."));
	}
}

void AWallActor::DestroyPreviousWalls() {
	for (auto it : UpperWallArray) {
		if (it) {
			it->DestroyComponent();
		}
	}
	UpperWallArray.Empty();

	for (auto it : LowerWallArray) {
		if (it) {
			it->DestroyComponent();
		}
	}
	LowerWallArray.Empty();
}

void AWallActor::ApplyMaterial(UMaterialInstance* WallMaterial_)
{
	WallMaterial = WallMaterial_;
	TArray<UStaticMeshComponent*> WallComponents;
	this->GetComponents(WallComponents);

	for (auto Component : WallComponents) {
		if (!Component->GetName().Contains("Door")) {
			Component->SetMaterial(0, WallMaterial_);
		}
		if (Component->GetName().Contains("Window1"))
		{
			Component->SetMaterial(1, WallMaterial_);
			GEngine->AddOnScreenDebugMessage(-1,2,FColor::Black,"Window1");
		}

	}
}

int32 AWallActor::GetNumSegments() const {
	return NumberofSegments;
}

void AWallActor::BeginPlay()
{
	Super::BeginPlay();
}

void AWallActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

