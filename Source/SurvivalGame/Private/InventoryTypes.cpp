// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryTypes.h"
#include "Inventory/DroppedItem_Basic.h"
#include "DEPRECATED/BasicInventoryReserverComponent.h"
#include "DEPRECATED/BasicItemData.h"
#include "DEPRECATED/BasicTool.h"
//#include "Engine/DataTable.h"
#include "Inventory/InventorySlotData_Stack.h"
//#include "Utility/UtilityFunctionLibrary.h"



FItemData_Simple::FItemData_Simple(UDataTable* DataTable, const FName RowName, const int32 NewAmount)
{
	StaticDataHandle = FDataTableRowHandle();
	if (DataTable) {
		StaticDataHandle.DataTable	= DataTable;
		StaticDataHandle.RowName	= RowName;
		Amount						= NewAmount;
	}
	Type	= -1;
	Subtype	= -1;
}

FItemData_Simple::FItemData_Simple(const uint8 NewType, const uint8 NewSubtype, const int32 NewAmount)
{
	StaticDataHandle = FDataTableRowHandle();
	Type	= NewType;
	Subtype = NewSubtype;
	Amount	= NewAmount;
}

bool FItemData_Simple::IsValid(const FItemData_Simple& Data)
{
	if (Data.StaticDataHandle.IsNull()) {
		return (Data.Type != -1 && Data.Subtype != -1);
	}
	if (Data.StaticDataHandle.DataTable->FindRowUnchecked(Data.StaticDataHandle.RowName)) return true;
	return false;
}

bool FItemData_Simple::operator==(FItemData_Simple const& OtherData) const
{
	return (StaticDataHandle == OtherData.StaticDataHandle) || (OtherData.Type == Type && OtherData.Subtype == Subtype);
}

bool FItemData_Simple::operator!=(FItemData_Simple const& OtherData) const
{
	return !(*this == OtherData);
}

FItemSlot::FItemSlot(UDataTable* DataTable, const FName RowName, const int32 NewAmount, UBasicItemData* NewInstancedDataRef)
{
	StaticDataHandle			= FDataTableRowHandle();
	StaticDataHandle.DataTable	= DataTable;
	StaticDataHandle.RowName	= RowName;
	Amount						= NewAmount;
	InstancedDataRef			= NewInstancedDataRef;
}

bool FItemSlot::IsValid(const FItemSlot ItemData)
{
	if (ItemData.Amount <= 0.0f) {
		return false;
	}
	try {
		//UUtilityFunctionLibrary::PrintDebug("Getting Row");
		if (ItemData.StaticDataHandle.GetRow<FItemStaticData>("")) {
			//UUtilityFunctionLibrary::PrintDebug("Got Row");
			return true;
		}
	}
	catch (...) {}
	return false;
}

/*
inline bool FItemStaticData::IsValid()
{
	return SpawnedClass && DisplayName != FItemStaticData::DisplayName && Icon;
}
//*/

bool FItemStaticData::IsValid(const FItemStaticData StaticData)
{
	return StaticData.DisplayName != "DEFAULT_NAME" && StaticData.Description != "DEFAULT_DESC" && StaticData.Icon;
}

bool FReservedInfo::IsValid(const FReservedInfo ReservedSlot)
{
	return ReservedSlot.NumSlots > 0 && ReservedSlot.ReserverComponentRef && !ReservedSlot.ReserverComponentRef->IsPendingKill();
}

FItemTypeRestriction::FItemTypeRestriction(uint8 NewTypeID, bool InvertRestriction) {
	TypeID = NewTypeID;
	IsInverted = InvertRestriction;
}

bool FItemTypeRestriction::IsMatching(const uint8 ItemTypeID, bool UseBitmask) const {
	if(TypeID == -1){
		return (TypeID == ItemTypeID) != IsInverted;
	}
	return (UseBitmask ? ((TypeID & ItemTypeID) != 0) : (ItemTypeID == TypeID)) != IsInverted;
}

bool FItemTypeRestriction::IsAllMatching(const uint8 ItemType, const uint8 ItemSubtype, const FItemTypeRestriction TypeRestriction, const FItemTypeRestriction SubtypeRestriction)
{
	if (!TypeRestriction.IsMatching(ItemType)) return false;
	if (TypeRestriction.TypeID == -1) return !TypeRestriction.IsInverted;
	if (SubtypeRestriction.TypeID == -1) return !SubtypeRestriction.IsInverted;
	switch (EItemType(TypeRestriction.TypeID)) {
		case EItemType::Component:
			return SubtypeRestriction.IsMatching(ItemSubtype, true);
			break;
		default:
			return SubtypeRestriction.IsMatching(ItemSubtype);
			break;
	}
}
