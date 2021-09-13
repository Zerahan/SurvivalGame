// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventorySlot_Tool.h"
#include "InventoryTypes.h"
#include "Inventory/InventorySlot_Stack.h"
#include "Inventory/InventoryManager.h"
//#include "Inventory/InventorySlot_Basic.h"


//********//
// STRUCT //
//********//

FInventorySlotData_Tool::FInventorySlotData_Tool(UDataTable* DataTable, const FName RowName) {
	StaticDataHandle			= FDataTableRowHandle();
	StaticDataHandle.DataTable	= DataTable;
	StaticDataHandle.RowName	= RowName;
}

FItemStaticData_Tool::FItemStaticData_Tool()
{
	DisplayName			= "DEFAULT_NAME";
	Description			= "DEFAULT_DESC";
	Icon				= nullptr;
	SpawnedClass		= nullptr;
	InstancedDataClass	= UInventorySlot_Tool::StaticClass();
}

bool FItemStaticData_Tool::IsValid(const FItemStaticData_Tool& StaticData)
{
	return StaticData.DisplayName != "DEFAULT_NAME";
}


//*******//
// CLASS //
//*******//

UInventorySlot_Tool::UInventorySlot_Tool()
{
	if (!GetStaticDataHandle().IsNull()) {
		FItemStaticData_Tool StaticData = *GetStaticDataHandle().GetRow<FItemStaticData_Tool>("Initializing InventorySlot_Tool from GetStaticDataHandle()");
		if (FItemStaticData_Tool::IsValid(StaticData)) {
			InternalSlots.AddDefaulted(StaticData.InternalSlotRestrictions.Num());
		}
	}
	AmmoSlotRef = nullptr;
}

void UInventorySlot_Tool::SetFromData(const FInventorySlotData_Tool NewSlotData)
{
	SetStaticDataHandle(NewSlotData.StaticDataHandle);
	if (NewSlotData.InternalSlots.Num() > 0) {
		InternalSlots.Empty();
		for (int i = 0; i < NewSlotData.InternalSlots.Num(); i++) {
			UInventorySlot_Stack* NewItem = UInventorySlot_Stack::CreateNewSlotFromHandle(NewSlotData.InternalSlots[i].StaticDataHandle, this);
			NewItem->SetFromData(NewSlotData.InternalSlots[i]);
			InternalSlots.Add(NewItem);
		}
	}
}

FItemStaticData_Tool UInventorySlot_Tool::LookupStaticData(UInventorySlot_Tool* SlotToFind)
{
	if (!SlotToFind->GetStaticDataHandle().IsNull()) {
		FItemStaticData_Tool StaticData = *SlotToFind->GetStaticDataHandle().GetRow<FItemStaticData_Tool>("UInventorySlot_Stack looking for static data");
		if (FItemStaticData_Tool::IsValid(StaticData)) {
			return StaticData;
		}
	}
	return FItemStaticData_Tool();
}

bool UInventorySlot_Tool::ShouldDestroyObject() const
{
    return false;
}

bool UInventorySlot_Tool::AddData(const FItemData_Simple& DataToAdd, FItemData_Simple& Remainder)
{
	for (int i = 0; i < InternalSlots.Num(); i++) {
		FItemData_Simple WorkingData = DataToAdd;
		InternalSlots[i]->AddData(WorkingData, Remainder);
		WorkingData = Remainder;
		if (Remainder.Amount <= 0) {
			return false;
		}
	}
	return Remainder.Amount > 0;
}

bool UInventorySlot_Tool::RemoveData(const FItemData_Simple& DataToRemove, FItemData_Simple& Remainder)
{
	for (int i = 0; i < InternalSlots.Num(); i++) {
		FItemData_Simple WorkingData = DataToRemove;
		InternalSlots[i]->RemoveData(WorkingData, Remainder);
		WorkingData = Remainder;
		if (Remainder.Amount <= 0) {
			return false;
		}
	}
	return Remainder.Amount > 0;
}

bool UInventorySlot_Tool::CanMergeData(UInventorySlot_Basic* OtherSlot) const
{
	if (!IsValid(OtherSlot)) { return false; }
	FItemStaticData_Stack OtherStaticData = *OtherSlot->GetStaticDataHandle().GetRow<FItemStaticData_Stack>("CanMergeData on InventorySlot_Tool");
	if (FItemStaticData_Stack::IsValid(OtherStaticData)) {
		FItemStaticData_Tool StaticData = *GetStaticDataHandle().GetRow<FItemStaticData_Tool>("CanMergeData on InventorySlot_Tool");
		if (StaticData.InternalSlotRestrictions.Contains(OtherStaticData.Subtype)) {
			return true;
		}
	}
	return false;
}

bool UInventorySlot_Tool::MergeData(UInventorySlot_Basic* OtherSlot, int32& AmountOverride)
{
	if (AmountOverride == -1 && CanMergeData(OtherSlot)) {
		bool HasMerged = false;
		FItemStaticData_Tool StaticData = *GetStaticDataHandle().GetRow<FItemStaticData_Tool>("CanMergeData on InventorySlot_Tool");
		FItemStaticData_Stack OtherStaticData = *OtherSlot->GetStaticDataHandle().GetRow<FItemStaticData_Stack>("CanMergeData on InventorySlot_Tool");
		for (int i = 0; i < InternalSlots.Num(); i++) {
			if (StaticData.InternalSlotRestrictions[i].Contains(OtherStaticData.Subtype)) {
				//UInventoryManager* MyManager = Cast<UInventoryManager>(GetOuter());
				UInventoryManager* OtherManager = Cast<UInventoryManager>(OtherSlot->GetOuter());
				OtherManager->SetSlot(OtherManager->GetIndex(OtherSlot), InternalSlots[i]);
				InternalSlots[i] = OtherSlot;
				InternalSlots[i]->Rename(nullptr, this);
				HasMerged = true;
				//if (!IsValid(AmmoSlotRef)) {
					if (StaticData.InternalSlotRestrictions[i].Contains(StaticData.AmmoSubtype)) {
						if (IsValid(InternalSlots[i])) {
							AmmoSlotRef = Cast<UInventorySlot_Stack>(InternalSlots[i]);
						}
						else {
							AmmoSlotRef = nullptr;
						}
					}
				//}
			}
		}
		return HasMerged;
	}
	return false;
}

int32 UInventorySlot_Tool::GetAmmoCount() const
{
	if (IsValid(AmmoSlotRef)) {
		return AmmoSlotRef->GetAmount();
	}
	return -1;
}

bool UInventorySlot_Tool::ConsumeAmmo(const FItemData_Simple AmmoData)
{
	if (!IsValid(AmmoSlotRef)) { return false; }
	FItemData_Simple Remainder;
	if (AmmoData.Amount < 0) {
		FItemData_Simple WorkingData = AmmoData;
		WorkingData.Amount = FMath::Abs(WorkingData.Amount);
		AmmoSlotRef->RemoveData(WorkingData, Remainder);
	}
	else if (AmmoData.Amount < 0) {
		AmmoSlotRef->AddData(AmmoData, Remainder);
	}
	return false;
}

UInventorySlot_Tool* UInventorySlot_Tool::CreateNewSlotFromHandle(const FDataTableRowHandle Handle, UObject* NewOuter)
{
	if (Handle.IsNull() || !IsValid(NewOuter)) { return nullptr; }
	FItemStaticData_Tool StaticData = *Handle.GetRow<FItemStaticData_Tool>("");
	if (FItemStaticData_Tool::IsValid(StaticData)) {
		UInventorySlot_Tool* NewSlot = NewObject<UInventorySlot_Tool>(NewOuter, StaticData.InstancedDataClass);
		NewSlot->SetStaticDataHandle(Handle);
		return NewSlot;
	}
	return nullptr;
}
