// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventorySlotData_Tool.h"
#include "Inventory/InventoryTypes.h"
#include "Inventory/InventorySlotData_Stack.h"
#include "Inventory/InventoryManager.h"
#include "Inventory/DroppedItem_Tool.h"
//#include "Inventory/InventorySlot_Basic.h"

//********//
// STRUCT //
//********//

FItemSlotBuilder_Tool::FItemSlotBuilder_Tool(UDataTable* DataTable, const FName RowName) {
	StaticDataHandle			= FDataTableRowHandle();
	StaticDataHandle.DataTable	= DataTable;
	StaticDataHandle.RowName	= RowName;
}

bool FItemSlotBuilder_Tool::IsValid() const
{
	if (StaticDataHandle.IsNull()) return false;
	if (StaticDataHandle.DataTable->FindRowUnchecked(StaticDataHandle.RowName)) return true;
	return false;
}

FItemStaticData_Tool::FItemStaticData_Tool()
{
	BasicData						= FItemStaticData_Basic();

	AmmoSubtype			= EAmmoSubtype::None;
	AmmoConsumedPerUse	= 0;
	EquippedClass		= nullptr;
}

bool FItemStaticData_Tool::IsValid(const FItemStaticData_Tool& StaticData)
{
	return FItemStaticData_Basic::IsValid(StaticData.BasicData);
}


//*******//
// CLASS //
//*******//

UInventorySlotData_Tool::UInventorySlotData_Tool()
{
	if (!GetStaticDataHandle().IsNull()) {
		FItemStaticData_Tool StaticData = *GetStaticDataHandle().GetRow<FItemStaticData_Tool>("Initializing InventorySlot_Tool from GetStaticDataHandle()");
		if (FItemStaticData_Tool::IsValid(StaticData)) {
			//InternalSlots.AddDefaulted(StaticData.InternalSlotRestrictions.Num());
		}
	}
	AmmoSlotRef = nullptr;
}

void UInventorySlotData_Tool::SetFromData(const FItemSlotBuilder_Tool& NewSlotData)
{
	SetStaticDataHandle(NewSlotData.StaticDataHandle);
	if (NewSlotData.InternalSlots.Num() > 0) {
		InternalSlots.Empty();
		for (int i = 0; i < NewSlotData.InternalSlots.Num(); i++) {
			if (NewSlotData.InternalSlots[i].IsValid()) {
				UInventorySlotData_Stack* NewItem = UInventorySlotData_Stack::CreateNewSlotFromHandle(NewSlotData.InternalSlots[i].StaticDataHandle, this);
				NewItem->SetFromData(NewSlotData.InternalSlots[i]);
				InternalSlots.Add(NewItem);
			}
			else {
				InternalSlots.Add(nullptr);
			}
		}
	}
}

FItemStaticData_Tool UInventorySlotData_Tool::LookupStaticData(UInventorySlotData_Basic* SlotToFind)
{
	FItemStaticData_Tool ReturnedStaticData = FItemStaticData_Tool();
	if (!IsValid(SlotToFind)) return ReturnedStaticData;
	if (SlotToFind->GetStaticDataHandle().IsNull()) return ReturnedStaticData;
	ReturnedStaticData = *SlotToFind->GetStaticDataHandle().GetRow<FItemStaticData_Tool>("InventorySlotData_Basic attempting to get data table row...");
	return ReturnedStaticData;
	/*
	if (!SlotToFind->GetStaticDataHandle().IsNull()) {
		FItemStaticData_Tool StaticData = *SlotToFind->GetStaticDataHandle().GetRow<FItemStaticData_Tool>("UInventorySlotData_Stack looking for static data");
		if (FItemStaticData_Tool::IsValid(StaticData)) {
			return StaticData;
		}
	}
	return FItemStaticData_Tool();
	//*/
}

bool UInventorySlotData_Tool::ShouldDestroyObject() const
{
    return false;
}

bool UInventorySlotData_Tool::AddData(const FItemData_Simple& DataToAdd, FItemData_Simple& Remainder)
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

bool UInventorySlotData_Tool::RemoveData(const FItemData_Simple& DataToRemove, FItemData_Simple& Remainder)
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

bool UInventorySlotData_Tool::CanMergeData(UInventorySlotData_Basic* OtherSlot) const
{
	if (!IsValid(OtherSlot)) { return false; }
	FItemStaticData_Stack OtherStaticData = *OtherSlot->GetStaticDataHandle().GetRow<FItemStaticData_Stack>("CanMergeData on InventorySlot_Tool");
	if (FItemStaticData_Stack::IsValid(OtherStaticData)) {
		FItemStaticData_Tool StaticData = *GetStaticDataHandle().GetRow<FItemStaticData_Tool>("CanMergeData on InventorySlot_Tool");
		//if (StaticData.InternalSlotRestrictions.Contains(OtherStaticData.SubTypeID)) {
			return true;
		//}
	}
	return false;
}

bool UInventorySlotData_Tool::MergeData(UInventorySlotData_Basic* OtherSlot, int32& AmountOverride)
{
	if (AmountOverride == -1 && CanMergeData(OtherSlot)) {
		bool HasMerged = false;
		FItemStaticData_Tool StaticData = *GetStaticDataHandle().GetRow<FItemStaticData_Tool>("CanMergeData on InventorySlot_Tool");
		FItemStaticData_Stack OtherStaticData = *OtherSlot->GetStaticDataHandle().GetRow<FItemStaticData_Stack>("CanMergeData on InventorySlot_Tool");
		for (int i = 0; i < InternalSlots.Num(); i++) {
			//if (StaticData.InternalSlotRestrictions.Contains(OtherStaticData.SubTypeID)) {
				//UInventoryManager* MyManager = Cast<UInventoryManager>(GetOuter());
				UInventoryManager* OtherManager = Cast<UInventoryManager>(OtherSlot->GetOuter());
				OtherManager->SetSlot(OtherManager->GetIndex(OtherSlot), InternalSlots[i]);
				InternalSlots[i] = OtherSlot;
				InternalSlots[i]->Rename(nullptr, this);
				HasMerged = true;
				//if (!IsValid(AmmoSlotRef)) {
					//if (StaticData.InternalSlotRestrictions[i].Contains(StaticData.AmmoSubtype)) {
						if (IsValid(InternalSlots[i])) {
							AmmoSlotRef = Cast<UInventorySlotData_Stack>(InternalSlots[i]);
						}
						else {
							AmmoSlotRef = nullptr;
						}
					//}
				//}
			//}
		}
		return HasMerged;
	}
	return false;
}

int32 UInventorySlotData_Tool::GetAmmoCount() const
{
	if (IsValid(AmmoSlotRef)) {
		return AmmoSlotRef->GetAmount();
	}
	return -1;
}

bool UInventorySlotData_Tool::ConsumeAmmo(const FItemData_Simple AmmoData)
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

UInventorySlotData_Tool* UInventorySlotData_Tool::CreateNewSlotFromHandle(const FDataTableRowHandle Handle, UObject* NewOuter)
{
	if (Handle.IsNull() || !IsValid(NewOuter)) { return nullptr; }
	FItemStaticData_Tool StaticData = *Handle.GetRow<FItemStaticData_Tool>("");
	if (FItemStaticData_Tool::IsValid(StaticData)) {
		UInventorySlotData_Tool* NewSlot = NewObject<UInventorySlotData_Tool>(NewOuter, (StaticData.BasicData.InstancedDataClass != nullptr ? StaticData.BasicData.InstancedDataClass : UInventorySlotData_Tool::StaticClass()));
		NewSlot->SetStaticDataHandle(Handle);
		return NewSlot;
	}
	return nullptr;
}
//*/
