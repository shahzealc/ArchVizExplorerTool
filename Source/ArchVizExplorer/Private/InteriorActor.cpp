// Fill out your copyright notice in the Description page of Project Settings.


#include "InteriorActor.h"

// Sets default values
AInteriorActor::AInteriorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	RootComponent = RootScene;

}

// Called when the game starts or when spawned
void AInteriorActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteriorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteriorActor::GenerateInterior(UStaticMesh* InteriorMesh,FString InteriorType)
{

	InteriorActorMesh = InteriorMesh;
	InteriorActorType = InteriorType;

	UStaticMeshComponent* Interior = NewObject<UStaticMeshComponent>(this, *InteriorType);
	Interior->AttachToComponent(RootScene, FAttachmentTransformRules::KeepRelativeTransform);

	Interior->RegisterComponentWithWorld(GetWorld());

	Interior->SetStaticMesh(InteriorMesh);

}

