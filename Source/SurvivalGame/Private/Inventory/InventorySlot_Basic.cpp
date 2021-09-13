// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventorySlot_Basic.h"
#include "InventoryTypes.h"

FItemStaticData_Basic::FItemStaticData_Basic()
{
	DisplayName			= "DEFAULT_NAME";
	Description			= "DEFAULT_DESC";
	Icon				= nullptr;
	SpawnedClass		= nullptr;
	InstancedDataClass	= UInventorySlot_Basic::StaticClass();
}

bool FItemStaticData_Basic::IsValid(const FItemStaticData_Basic& StaticData)
{
	return StaticData.DisplayName != "DEFAULT_NAME";
}

FDataTableRowHandle UInventorySlot_Basic::GetStaticDataHandle() const
{
	return StaticDataHandle;
}

void UInventorySlot_Basic::SetStaticDataHandle(const FDataTableRowHandle& NewStaticDataHandle)
{
	StaticDataHandle = NewStaticDataHandle;
}

FItemStaticData_Basic UInventorySlot_Basic::LookupStaticData(UInventorySlot_Basic* SlotToFind)
{
	if (!SlotToFind->GetStaticDataHandle().IsNull()) {
		FItemStaticData_Basic StaticData = *SlotToFind->GetStaticDataHandle().GetRow<FItemStaticData_Basic>("UInventorySlot_Basic looking for static data");
		if (FItemStaticData_Basic::IsValid(StaticData)) {
			return StaticData;
		}
	}
	return FItemStaticData_Basic();
}

bool UInventorySlot_Basic::AddData(const FItemData_Simple& DataToAdd, FItemData_Simple& Remainder) {
	// Can be used to validate incoming data for subclasses
	if (FItemData_Simple::IsValid(DataToAdd) && DataToAdd.StaticDataHandle == StaticDataHandle) {
		return false;
	}
	Remainder = DataToAdd;
	return true;
}

bool UInventorySlot_Basic::RemoveData(const FItemData_Simple& DataToRemove, FItemData_Simple& Remainder) {
	// Can be used to validate subclasses
	if (FItemData_Simple::IsValid(DataToRemove) && DataToRemove.StaticDataHandle == StaticDataHandle) {
		return false;
	}
	Remainder = DataToRemove;
	return true;
}

bool UInventorySlot_Basic::CanMergeData(UInventorySlot_Basic* OtherSlot) const
{
	return IsValid(OtherSlot);
}

bool UInventorySlot_Basic::MergeData(UInventorySlot_Basic* OtherSlot, int32& AmountOverride)
{
	// Can be used to validate incoming data for subclasses
	if (IsValid(OtherSlot) && OtherSlot->StaticDataHandle == StaticDataHandle) {
		return true;
	}
	return false;
}

bool UInventorySlot_Basic::ShouldDestroyObject() const
{
	return true;
}

UInventorySlot_Basic* UInventorySlot_Basic::CreateNewSlotFromHandle(const FDataTableRowHandle Handle, UObject* NewOuter)
{
	return nullptr;
}
