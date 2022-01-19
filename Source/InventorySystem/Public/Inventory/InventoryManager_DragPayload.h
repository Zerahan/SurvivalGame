// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryManager_DragPayload.generated.h"

class UInventoryManager;
class UInventorySlotData_Basic;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UInventoryManager_DragPayload : public UObject
{
	GENERATED_BODY()
public:
	UInventoryManager_DragPayload();
	
	/*
	* The inventory that an item is being pulled from. Optional: Defaults to nullptr. Only used when transferring items from one inventory to another.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory", meta = (ExposeOnSpawn = "true"))
	UInventoryManager* FromInventoryManagerRef;

	/*
	* The slot that an item is being pulled from. Must be set if FromInventoryManagerRef has been set. Otherwise defaults to -1.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory", meta = (ExposeOnSpawn = "true"))
	int32 FromInventorySlot;

	/*
	* The inventory that an item is being pushed into. If not set, the transfer will spawn one or more dropped versions of the item in the world.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory", meta = (ExposeOnSpawn = "true"))
	UInventoryManager* ToInventoryManagerRef;

	/*
	* The slot that an item is being pushed into. Optional: Defaults to -1. If default, the item will be pushed into the first available slot within ToInventory.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory", meta = (ExposeOnSpawn = "true"))
	int32 ToInventorySlot;

	/*
	* The actual data within the slot that is being transferred. Used primarily for bringing in items from non-inventory sources, IE: harvesting, picking up dropped loot, etc.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory", meta = (ExposeOnSpawn = "true"))
	UInventorySlotData_Basic* BasicSlotRef;

	/*
	* The specific amount of the item to transfer. Optional: Defaults to -1. If default, transfer everything within the slot.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory", meta = (ExposeOnSpawn = "true"))
	float AmountOverride;
};
