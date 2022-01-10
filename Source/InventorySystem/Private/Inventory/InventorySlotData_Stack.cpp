// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventorySlotData_Stack.h"
#include "Inventory/DroppedItem_Generic.h"
#include "Inventory/InventoryTypes.h"



//********//
// STRUCT //
//********//

FItemSlotBuilder_Stack::FItemSlotBuilder_Stack(UDataTable* DataTable, const FName RowName, const int32 NewAmount) {
    StaticDataHandle            = FDataTableRowHandle();
    StaticDataHandle.DataTable  = DataTable;
    StaticDataHandle.RowName    = RowName;
    Amount                      = NewAmount;
}

bool FItemSlotBuilder_Stack::IsValid() const
{
    if(StaticDataHandle.IsNull()) return false;
    if(StaticDataHandle.DataTable->FindRowUnchecked(StaticDataHandle.RowName)) return true;
    return false;
}

FItemStaticData_Stack::FItemStaticData_Stack()
{
    BasicData                       = FItemStaticData_Basic();

    MaxStackSize        = 1;
    StackCompressSize   = 1;
    IsContainer         = false;
    AbilityClass        = nullptr;
}

bool FItemStaticData_Stack::IsValid(const FItemStaticData_Stack& StaticData)
{
    return FItemStaticData_Basic::IsValid(StaticData.BasicData);
}



//*******//
// CLASS //
//*******//

void UInventorySlotData_Stack::SetAmount(const int32 NewAmount)
{
    Amount = NewAmount;
}

void UInventorySlotData_Stack::SetFromData(const FItemSlotBuilder_Stack& NewSlotData)
{
    SetStaticDataHandle(NewSlotData.StaticDataHandle);
    Amount = NewSlotData.Amount;
}

bool UInventorySlotData_Stack::AddData(const FItemData_Simple& DataToAdd, FItemData_Simple& Remainder)
{
    if (DataToAdd.StaticDataHandle != GetStaticDataHandle()) {
        Remainder = DataToAdd;
        return true;
    }
    FItemStaticData_Stack StaticData = UInventorySlotData_Stack::LookupStaticData(this);
    if (FItemStaticData_Stack::IsValid(StaticData)) {
        if (Amount + DataToAdd.Amount <= StaticData.MaxStackSize) {
            Amount += DataToAdd.Amount;
            return false;
        }
        else {
            Remainder = DataToAdd;
            Remainder.Amount -= StaticData.MaxStackSize - Amount;
            Amount = StaticData.MaxStackSize;
            return true;
        }
    }
    return false;
}

bool UInventorySlotData_Stack::RemoveData(const FItemData_Simple& DataToRemove, FItemData_Simple& Remainder)
{
    if (DataToRemove.StaticDataHandle != GetStaticDataHandle()) {
        Remainder = DataToRemove;
        return true;
    }
    if (Amount - DataToRemove.Amount >= 0) {
        Amount -= DataToRemove.Amount;
        return false;
    }
    else {
        Remainder = DataToRemove;
        Remainder.Amount -= Amount;
        Amount = 0;
        return true;
    }
    return false;
}

bool UInventorySlotData_Stack::CanMergeData(UInventorySlotData_Basic* OtherSlot) const
{
    if (!Super::CanMergeData(OtherSlot)) { return false; }
    if (!IsValid(Cast<UInventorySlotData_Stack>(OtherSlot))) { return false; }
    //UInventorySlotData_Stack* Slot = ;
    return GetStaticDataHandle() == OtherSlot->GetStaticDataHandle();
}

bool UInventorySlotData_Stack::MergeData(UInventorySlotData_Basic* OtherSlot, int32& AmountOverride)
{
    if (AmountOverride == 0) { return false; }
    if (!Super::MergeData(OtherSlot, AmountOverride)) { return false; }
    FItemStaticData_Stack StaticData = UInventorySlotData_Stack::LookupStaticData(this);
    //if (!StaticData.IsTransferrable) { return false; }
    UInventorySlotData_Stack* OtherSlotRef = Cast<UInventorySlotData_Stack>(OtherSlot);
    if (OtherSlotRef && FItemStaticData_Stack::IsValid(StaticData)) {
        float MoveAmount = OtherSlotRef->GetAmount();
        if(AmountOverride > 0 && AmountOverride <= OtherSlotRef->GetAmount()){
            MoveAmount = AmountOverride;
        }
        if (Amount + MoveAmount <= StaticData.MaxStackSize) {
            Amount += MoveAmount;
            return true;
        }
        else {
            MoveAmount = (StaticData.MaxStackSize - Amount);
            Amount = StaticData.MaxStackSize;
        }
        OtherSlotRef->SetAmount(OtherSlotRef->GetAmount() - MoveAmount);
        if (AmountOverride > 0) { AmountOverride -= MoveAmount; }
        return true;
    }
    return false;
}

bool UInventorySlotData_Stack::ShouldDestroyObject() const
{
    // If the object is a container, don't destroy when amount is 0.
    FItemStaticData_Stack StaticData = UInventorySlotData_Stack::LookupStaticData(this);
    if (FItemStaticData_Stack::IsValid(StaticData)) {
        if (StaticData.IsContainer) { return false; }
    }
    return Amount == 0;
}

FItemStaticData_Stack UInventorySlotData_Stack::LookupStaticData(const UInventorySlotData_Basic* SlotToFind)
{
    FItemStaticData_Stack ReturnedStaticData = FItemStaticData_Stack();
    if (!IsValid(SlotToFind)) return ReturnedStaticData;
    if (SlotToFind->GetStaticDataHandle().IsNull()) return ReturnedStaticData;
    ReturnedStaticData = *SlotToFind->GetStaticDataHandle().GetRow<FItemStaticData_Stack>("InventorySlotData_Basic attempting to get data table row...");
    return ReturnedStaticData;
}

UInventorySlotData_Stack* UInventorySlotData_Stack::CreateNewSlotFromHandle(const FDataTableRowHandle Handle, UObject* NewOuter)
{
    if (Handle.IsNull() || !IsValid(NewOuter)) { return nullptr; }
    FItemStaticData_Stack StaticData = *Handle.GetRow<FItemStaticData_Stack>("");
    if (FItemStaticData_Stack::IsValid(StaticData)) {
        UInventorySlotData_Stack* NewSlot = NewObject<UInventorySlotData_Stack>(NewOuter, (StaticData.BasicData.InstancedDataClass != nullptr ? StaticData.BasicData.InstancedDataClass : UInventorySlotData_Stack::StaticClass()));
        NewSlot->SetStaticDataHandle(Handle);
        return NewSlot;
    }
    return nullptr;
}
