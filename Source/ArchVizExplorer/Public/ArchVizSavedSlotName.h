// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ArchVizSavedSlotName.generated.h"

USTRUCT()
struct FSlotNames
{
	GENERATED_BODY()

	UPROPERTY()
	FString SlotName;

};


UCLASS()
class ARCHVIZEXPLORER_API UArchVizSavedSlotName : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<FSlotNames> SlotNames;
};
