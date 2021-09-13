// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventorySlot_Stack.h"
#include "InventoryTypes.h"



//********//
// STRUCT //
//********//

FInventorySlotData_Stack::FInventorySlotData_Stack(UDataTable* DataTable, const FName RowName, const int32 NewAmount) {
    StaticDataHandle            = FDataTableRowHandle();
    StaticDataHandle.DataTable  = DataTable;
    StaticDataHandle.RowName    = RowName;
    Amount                      = NewAmount;
}

bool FInventorySlotData_Stack::IsValid() const
{
    return !StaticDataHandle.IsNull();
}

FItemStaticData_Stack::FItemStaticData_Stack()
{
    DisplayName         = "DEFAULT_NAME";
    Description         = "DEFAULT_DESC";
    Icon                = nullptr;
    MaxStackSize        = 1;
    StackCompressSize   = 1;
    Subtype             = "None";
    AbilityClass        = nullptr;
    SpawnedClass        = nullptr;
    InstancedDataClass  = UInventorySlot_Stack::StaticClass();
}

bool FItemStaticData_Stack::IsValid(const FItemStaticData_Stack& StaticData)
{
    return StaticData.DisplayName != "DEFAULT_NAME";
}



//*******//
// CLASS //
//*******//

UInventorySlot_Stack::UInventorySlot_Stack()
{
}

inline void UInventorySlot_Stack::SetAmount(const int32 NewAmount) { Amount = NewAmount; }

inline int32 UInventorySlot_Stack::GetAmount() const { return Amount; }

void UInventorySlot_Stack::SetStaticDataHandle(const FDataTableRowHandle& NewStaticDataHandle)
{
    Super::SetStaticDataHandle(NewStaticDataHandle);
}

FItemStaticData_Stack UInventorySlot_Stack::LookupStaticData(UInventorySlot_Stack* SlotToFind)
{
    if (!SlotToFind->GetStaticDataHandle().IsNull()) {
        FItemStaticData_Stack StaticData = *SlotToFind->GetStaticDataHandle().GetRow<FItemStaticData_Stack>("UInventorySlot_Stack looking for static data");
        if (FItemStaticData_Stack::IsValid(StaticData)) {
            return StaticData;
        }
    }
    return FItemStaticData_Stack();
}

void UInventorySlot_Stack::SetFromData(const FInventorySlotData_Stack NewSlotData)
{
    SetStaticDataHandle(NewSlotData.StaticDataHandle);
    Amount = NewSlotData.Amount;
}

bool UInventorySlot_Stack::AddData(const FItemData_Simple& DataToAdd, FItemData_Simple& Remainder)
{
    if (DataToAdd.StaticDataHandle != GetStaticDataHandle()) {
        Remainder = DataToAdd;
        return true;
    }
    FItemStaticData_Stack StaticData = LookupStaticData(this);
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

bool UInventorySlot_Stack::RemoveData(const FItemData_Simple& DataToRemove, FItemData_Simple& Remainder)
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

bool UInventorySlot_Stack::CanMergeData(UInventorySlot_Basic* OtherSlot) const
{
    if (!Super::CanMergeData(OtherSlot)) { return false; }
    if (!IsValid(Cast<UInventorySlot_Stack>(OtherSlot))) { return false; }
    //UInventorySlot_Stack* Slot = ;
    return GetStaticDataHandle() == OtherSlot->GetStaticDataHandle();
}

bool UInventorySlot_Stack::MergeData(UInventorySlot_Basic* OtherSlot, int32& AmountOverride)
{
    if (AmountOverride == 0) { return false; }
    if (!Super::MergeData(OtherSlot, AmountOverride)) { return false; }
    FItemStaticData_Stack StaticData = LookupStaticData(this);
    //if (!StaticData.IsTransferrable) { return false; }
    UInventorySlot_Stack* OtherSlotRef = Cast<UInventorySlot_Stack>(OtherSlot);
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

bool UInventorySlot_Stack::ShouldDestroyObject() const
{
    // If the object is a container, don't destroy when amount is 0.
    if (IsContainer) { return false; }
    return Amount == 0;
}

UInventorySlot_Stack* UInventorySlot_Stack::CreateNewSlotFromHandle(const FDataTableRowHandle Handle, UObject* NewOuter)
{
    if (Handle.IsNull() || !IsValid(NewOuter)) { return nullptr; }
    FItemStaticData_Stack StaticData = *Handle.GetRow<FItemStaticData_Stack>("");
    if (FItemStaticData_Stack::IsValid(StaticData)) {
        UInventorySlot_Stack* NewSlot = NewObject<UInventorySlot_Stack>(NewOuter, StaticData.InstancedDataClass);
        NewSlot->SetStaticDataHandle(Handle);
        return NewSlot;
    }
    return nullptr;
}
