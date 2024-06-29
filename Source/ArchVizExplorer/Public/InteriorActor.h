// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteriorActor.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API AInteriorActor : public AActor
{
	GENERATED_BODY()

public:
	AInteriorActor();

	UPROPERTY()
	USceneComponent* RootScene;

	UPROPERTY()
	UStaticMesh* InteriorActorMesh;
	
	UPROPERTY()
	FString InteriorActorType;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void GenerateInterior(UStaticMesh* InteriorMesh, FString Interior);

};
