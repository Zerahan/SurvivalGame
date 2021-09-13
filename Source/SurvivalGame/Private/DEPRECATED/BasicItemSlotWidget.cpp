// Fill out your copyright notice in the Description page of Project Settings.


#include "DEPRECATED/BasicItemSlotWidget.h"
#include "DEPRECATED/InventoryComponent.h"
#include "InventoryTypes.h"
#include "Utility/UtilityFunctionLibrary.h"

void UBasicItemSlotWidget::SetInventoryRef(UInventoryComponent * NewInventoryRef)
{
	InventoryRef = NewInventoryRef;
	UpdateGraphics();
}

void UBasicItemSlotWidget::SetTargetSlot(const int32 NewTargetSlot)
{
	TargetSlotIndex = NewTargetSlot;
	UpdateGraphics();
}

void UBasicItemSlotWidget::SetItemData(const FItemSlot NewItemData)
{
	ItemData = NewItemData;
	UpdateGraphics();
}

void UBasicItemSlotWidget::UpdateGraphics_Implementation()
{
}
