// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/WidgetInventorySlot_Basic.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/InventoryManager.h"
#include "Inventory/InventorySlot_Basic.h"
#include "Inventory/InventorySlot_Stack.h"
#include "Inventory/InventorySlot_Tool.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Utility/UtilityFunctionLibrary.h"

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

void UWidgetInventorySlot_Basic::SetDataFromSlot(UInventorySlot_Basic* NewManualSlotRef)
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
	UInventorySlot_Basic* BasicRef = nullptr;
	if (IsValid(InventoryRef)) {
		BasicRef = InventoryRef->GetSlot(TargetSlotIndex);
		//UUtilityFunctionLibrary::PrintDebug("Valid Index: " + FString::FromInt(TargetSlotIndex));
	}
	else if(IsValid(ManualSlotRef)) {
		BasicRef = ManualSlotRef;
	}
	if (IsValid(BasicRef)) {
		FDataTableRowHandle StaticDataHandle = BasicRef->GetStaticDataHandle();
		if (!StaticDataHandle.IsNull()) {
			if (auto StackRef = Cast<UInventorySlot_Stack>(BasicRef)) {
				FItemStaticData_Stack StaticData = *StaticDataHandle.GetRow<FItemStaticData_Stack>("InventorySlot UI");
				if (DisplayName.ToString() != StaticData.DisplayName) {
					DisplayName = FText::FromString(StaticData.DisplayName);
					Description = FText::FromString(StaticData.Description);
					IconTexture = StaticData.Icon.LoadSynchronous();
					CompressAmount = StaticData.StackCompressSize;
				}
				if (StaticData.MaxStackSize > 1) {
					Amount = StackRef->GetAmount();
				}
			}
			else if(auto ToolRef = Cast<UInventorySlot_Tool>(BasicRef)) {
				FItemStaticData_Tool StaticData = *StaticDataHandle.GetRow<FItemStaticData_Tool>("InventorySlot UI");
				if (DisplayName.ToString() != StaticData.DisplayName) {
					DisplayName = FText::FromString(StaticData.DisplayName);
					Description = FText::FromString(StaticData.Description);
					IconTexture = StaticData.Icon.LoadSynchronous();
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