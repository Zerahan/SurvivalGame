// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryTypes.h"
#include "Inventory/DroppedItem_Basic.h"
#include "DEPRECATED/BasicInventoryReserverComponent.h"
#include "DEPRECATED/BasicItemData.h"
#include "DEPRECATED/BasicTool.h"
//#include "Engine/DataTable.h"
#include "Inventory/InventorySlot_Stack.h"
//#include "Utility/UtilityFunctionLibrary.h"



FItemData_Simple::FItemData_Simple(UDataTable* DataTable, const FName RowName, const int32 NewAmount, const FString NewSubtype)
{
	if (DataTable) {
		StaticDataHandle = FDataTableRowHandle();
		StaticDataHandle.DataTable	= DataTable;
		StaticDataHandle.RowName	= RowName;
		Amount						= NewAmount;
		Subtype						= NewSubtype;
	}
}

bool FItemData_Simple::IsValid(const FItemData_Simple& Data)
{
	if (Data.Amount < 0) { return false; }
	try {
		if (Data.StaticDataHandle.GetRow<FTableRowBase>("Validating StaticDataHandle for FItemData_Simple")) {
			return true;
		}
	}
	catch (...) {}
	return false;
}

bool FItemData_Simple::operator==(FItemData_Simple const& OtherData) const
{
	return StaticDataHandle == OtherData.StaticDataHandle;
}

bool FItemData_Simple::operator!=(FItemData_Simple const& OtherData) const
{
	return StaticDataHandle != OtherData.StaticDataHandle;
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
