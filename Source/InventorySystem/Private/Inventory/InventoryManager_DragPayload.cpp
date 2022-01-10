// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryManager_DragPayload.h"
#include "Inventory/InventoryManager.h"
#include "Inventory/InventorySlotData_Basic.h"

UInventoryManager_DragPayload::UInventoryManager_DragPayload() {
	FromInventoryManagerRef	= nullptr;
	ToInventoryManagerRef	= nullptr;
	FromInventorySlot		= -1;
	ToInventorySlot			= -1;
	BasicSlotRef			= nullptr;
	AmountOverride			= -1.0f;
}