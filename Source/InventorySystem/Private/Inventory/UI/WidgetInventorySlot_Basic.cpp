// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/UI/WidgetInventorySlot_Basic.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/InventoryManager.h"
#include "Inventory/InventorySlotData_Basic.h"
#include "Inventory/InventorySlotData_Stack.h"
#include "Inventory/InventorySlotData_Tool.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UWidgetInventorySlot_Basic::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWidgetInventorySlot_Basic::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Execute_UpdateGraphics(this);
}

void UWidgetInventorySlot_Basic::OpenMenu_Implementation()
{
}

void UWidgetInventorySlot_Basic::Rebuild_Implementation()
{
}

void UWidgetInventorySlot_Basic::CloseMenu_Implementation()
{
}

void UWidgetInventorySlot_Basic::ToggleMenu_Implementation()
{
}

void UWidgetInventorySlot_Basic::SetDataFromInventory(UInventoryManager* NewInventoryRef, const int32 NewTargetSlot)
{
	TargetSlotIndex	= NewTargetSlot;
	InventoryRef	= NewInventoryRef;
	Execute_UpdateGraphics(this);
}

void UWidgetInventorySlot_Basic::SetDataFromSlot(UInventorySlotData_Basic* NewManualSlotRef)
{
	ManualSlotRef = NewManualSlotRef;
	Execute_UpdateGraphics(this);
}
void UWidgetInventorySlot_Basic::UpdateInput_Implementation()
{
}

UWidgetInventorySlot_Basic::UWidgetInventorySlot_Basic(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InventoryRef = nullptr;
	TargetSlotIndex = -1;
	DisplayName = FText::GetEmpty();
	Description = FText::GetEmpty();
	IconTexture = nullptr;
	Amount = -1;
	CompressAmount = -1;
	Health = -1;
}

void UWidgetInventorySlot_Basic::UpdateGraphics_Implementation()
{
	UInventorySlotData_Basic* BasicRef = nullptr;
	if (IsValid(InventoryRef)) {
		BasicRef = InventoryRef->GetSlot(TargetSlotIndex);
		//Debug("Valid Index: " + FString::FromInt(TargetSlotIndex));
	}
	else if(IsValid(ManualSlotRef)) {
		BasicRef = ManualSlotRef;
	}
	if (IsValid(BasicRef)) {
		FDataTableRowHandle StaticDataHandle = BasicRef->GetStaticDataHandle();
		if (!StaticDataHandle.IsNull()) {
			if (auto StackRef = Cast<UInventorySlotData_Stack>(BasicRef)) {
				FItemStaticData_Stack StaticData = *StaticDataHandle.GetRow<FItemStaticData_Stack>("InventorySlot UI");
				if (DisplayName.ToString() != StaticData.BasicData.DisplayName) {
					DisplayName = FText::FromString(StaticData.BasicData.DisplayName);
					Description = FText::FromString(StaticData.BasicData.Description);
					IconTexture = StaticData.BasicData.Icon.LoadSynchronous();
					CompressAmount = StaticData.StackCompressSize;
				}
				if (StaticData.MaxStackSize > 1) {
					Amount = StackRef->GetAmount();
				}
			}
			else if(auto ToolRef = Cast<UInventorySlotData_Tool>(BasicRef)) {
				FItemStaticData_Tool StaticData = *StaticDataHandle.GetRow<FItemStaticData_Tool>("InventorySlot UI");
				if (DisplayName.ToString() != StaticData.BasicData.DisplayName) {
					DisplayName = FText::FromString(StaticData.BasicData.DisplayName);
					Description = FText::FromString(StaticData.BasicData.Description);
					IconTexture = StaticData.BasicData.Icon.LoadSynchronous();
					Amount = -1;
					CompressAmount = -1;
				}
				Amount = ToolRef->GetAmmoCount();
			}
			return;
		}
	}
	DisplayName		= FText::GetEmpty();
	Description		= FText::GetEmpty();
	IconTexture		= nullptr;
	Amount			= -1;
	CompressAmount	= -1;
}