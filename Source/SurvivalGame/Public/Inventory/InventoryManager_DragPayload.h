// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryManager_DragPayload.generated.h"

class UInventoryManager;
class UInventorySlot_Basic;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SURVIVALGAME_API UInventoryManager_DragPayload : public UObject
{
	GENERATED_BODY()
public:
	UInventoryManager_DragPayload();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory", meta = (ExposeOnSpawn = "true"))
	UInventoryManager* FromInventoryManagerRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory", meta = (ExposeOnSpawn = "true"))
	int32 FromInventorySlot;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory", meta = (ExposeOnSpawn = "true"))
	UInventoryManager* ToInventoryManagerRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory", meta = (ExposeOnSpawn = "true"))
	int32 ToInventorySlot;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory", meta = (ExposeOnSpawn = "true"))
	UInventorySlot_Basic* BasicSlotRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory", meta = (ExposeOnSpawn = "true"))
	float AmountOverride;
};
