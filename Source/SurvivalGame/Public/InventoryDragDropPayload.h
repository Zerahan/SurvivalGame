// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryTypes.h"
#include "InventoryDragDropPayload.generated.h"

class UInventoryManager;

/**
 * Payload for drag/drop operations involving the InventoryComponent
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SURVIVALGAME_API UInventoryDragDropPayload : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory", meta = (ExposeOnSpawn = "true"))
	UInventoryManager* FromInventoryRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory", meta = (ExposeOnSpawn = "true"))
	UInventoryManager* ToInventoryRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory", meta = (ExposeOnSpawn = "true"))
	int32 FromSlot = -1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory", meta = (ExposeOnSpawn = "true"))
	int32 ToSlot = -1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory", meta = (ExposeOnSpawn = "true"))
	float AmountOverride = -1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory", meta = (ExposeOnSpawn = "true"))
	FItemData_Simple ItemData = FItemData_Simple();
};
