// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventorySlotData_Basic.h"
#include "Inventory/InventoryTypes.h"

FItemSlotBuilder_Basic::FItemSlotBuilder_Basic(UDataTable* DataTable, const FName RowName) {
	StaticDataHandle = FDataTableRowHandle();
	StaticDataHandle.DataTable = DataTable;
	StaticDataHandle.RowName = RowName;
}

bool FItemSlotBuilder_Basic::IsValid() const
{
	if (StaticDataHandle.IsNull()) return false;
	if (StaticDataHandle.DataTable->FindRowUnchecked(StaticDataHandle.RowName)) return true;
	return false;
}

FItemStaticData_Basic::FItemStaticData_Basic()
{
	DisplayName			= "DEFAULT_NAME";
	Description			= "DEFAULT_DESC";
	ItemType			= EItemType::None;
	ItemSubtype			= 0;
	Icon				= nullptr;
	InstancedDataClass	= nullptr;
	SpawnedClass		= nullptr;
}

bool FItemStaticData_Basic::IsValid(const FItemStaticData_Basic& StaticData)
{
	return StaticData.DisplayName != "DEFAULT_NAME";
}

void UInventorySlotData_Basic::SetFromData(const FItemSlotBuilder_Basic& NewSlotData)
{
	SetStaticDataHandle(NewSlotData.StaticDataHandle);
}

void UInventorySlotData_Basic::SetStaticDataHandle(const FDataTableRowHandle& NewStaticDataHandle)
{
	if (!StaticDataHandle.IsNull()) return;
	StaticDataHandle = NewStaticDataHandle;
}

FDataTableRowHandle UInventorySlotData_Basic::GetStaticDataHandle() const
{
	return StaticDataHandle;
}

bool UInventorySlotData_Basic::AddData(const FItemData_Simple& DataToAdd, FItemData_Simple& Remainder) {
	return FItemData_Simple::IsValid(DataToAdd);
}

bool UInventorySlotData_Basic::RemoveData(const FItemData_Simple& DataToRemove, FItemData_Simple& Remainder) {
	return FItemData_Simple::IsValid(DataToRemove);
}

bool UInventorySlotData_Basic::CanMergeData(UInventorySlotData_Basic* OtherSlot) const
{
	return IsValid(OtherSlot);
}

bool UInventorySlotData_Basic::MergeData(UInventorySlotData_Basic* OtherSlot, int32& AmountOverride)
{
	return false;
}

bool UInventorySlotData_Basic::ShouldDestroyObject() const
{
	return true;
}

FItemStaticData_Basic UInventorySlotData_Basic::LookupStaticData(UInventorySlotData_Basic* SlotToFind)
{
	FItemStaticData_Basic ReturnedStaticData = FItemStaticData_Basic();
	if (!IsValid(SlotToFind)) return ReturnedStaticData;
	if (SlotToFind->GetStaticDataHandle().IsNull()) return ReturnedStaticData;
	ReturnedStaticData = *SlotToFind->GetStaticDataHandle().GetRow<FItemStaticData_Basic>("InventorySlotData_Basic attempting to get data table row...");
	return ReturnedStaticData;
}

UInventorySlotData_Basic* UInventorySlotData_Basic::CreateNewSlotFromHandle(const FDataTableRowHandle Handle, UObject* NewOuter)
{
	if (Handle.IsNull() || !IsValid(NewOuter)) { return nullptr; }
	FItemStaticData_Basic StaticData = *Handle.GetRow<FItemStaticData_Basic>("");
	if (FItemStaticData_Basic::IsValid(StaticData)) {
		UInventorySlotData_Basic* NewSlot = NewObject<UInventorySlotData_Basic>(NewOuter, (StaticData.InstancedDataClass != nullptr ? StaticData.InstancedDataClass : UInventorySlotData_Basic::StaticClass()));
		NewSlot->SetStaticDataHandle(Handle);
		return NewSlot;
	}
	return nullptr;
}
