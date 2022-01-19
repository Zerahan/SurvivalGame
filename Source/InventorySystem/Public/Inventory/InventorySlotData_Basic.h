// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Inventory/InventoryTypes.h"
#include "InventorySlotData_Basic.generated.h"

class ADroppedItem_Basic;

// Serialize with FObjectWriter/FObjectReader


// All of the data that is identical between different instances of this item.
USTRUCT(BlueprintType)
struct INVENTORYSYSTEM_API FItemStaticData_Basic : public FTableRowBase {
	GENERATED_BODY()

	FItemStaticData_Basic();

	// The name of the item as shown to users. Replace with FText for localization at a later point.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName;

	// The name of the item as shown to users. Replace with FText for localization at a later point.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	// The overall itemtype for ambiguous data processing.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;

	// The subtype to refine ambiguous data processing.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 ItemSubtype;

	// The icon for this item as shown to users.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Icon;

	// The slot that is created when this item is added to an inventory.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UInventorySlotData_Basic> InstancedDataClass;

	// The actor that is spawned when this item is dropped into the world.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ADroppedItem_Basic> SpawnedClass;

	/*
	* Mostly just makes sure the data put in is not default data.
	* @param StaticData		The data to check.
	* @return				True if StaticData contains usable data.
	*/
	static bool IsValid(const FItemStaticData_Basic& StaticData);
};


//A struct that contains all of the data that this slot type can hold, used to construct the slot.
USTRUCT(BlueprintType)
struct FItemSlotBuilder_Basic {
	GENERATED_BODY()

public:
	FItemSlotBuilder_Basic(UDataTable* DataTable = nullptr, const FName RowName = NAME_None);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Inventory")
	FDataTableRowHandle StaticDataHandle;

	bool IsValid() const;
};


// The actual item that is processed during active gameplay.
UCLASS(BlueprintType, Blueprintable, Abstract, ClassGroup = (Custom))
class INVENTORYSYSTEM_API UInventorySlotData_Basic : public UObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default|Inventory", meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	FDataTableRowHandle StaticDataHandle;

public:
	/*
	* Sets the data for this slot from a SlotBuilder struct
	* @param NewSlotData The data for this slot.
	*/
	void SetFromData(const FItemSlotBuilder_Basic& NewSlotData);

	/*
	* Gets the Data table row handle to get static data for this slot.
	* @return	Datatable and row name
	*/
	UFUNCTION(BlueprintCallable)
	FDataTableRowHandle GetStaticDataHandle() const;

	/*
	* Sets where to find this item's static data.
	* @param NewStaticDataHandle	The handle used to lookup static data.
	*/
	UFUNCTION(BlueprintCallable)
	void SetStaticDataHandle(const FDataTableRowHandle& NewStaticDataHandle);

	/**
	* Adds simple data to the slot.
	* @param DataToAdd	The simple data to try to add to the slot.
	* @param Remainder	Contains leftover data when the operation is completed.
	* @return			True if Remainder given anything to contain.
	*/
	UFUNCTION(BlueprintCallable, Category = "Default|Inventory")
	virtual bool AddData(const FItemData_Simple& DataToAdd, FItemData_Simple& Remainder);

	/**
	* Removes simple data from the slot.
	* @param DataToRemove	The simpledata to try to remove from the slot.
	* @param Remainder		Contains leftover data when the operation is completed.
	* @return				True if Remainder given anything to contain.
	*/
	UFUNCTION(BlueprintCallable, Category = "Default|Inventory")
	virtual bool RemoveData(const FItemData_Simple& DataToRemove, FItemData_Simple& Remainder);

	/**
	* Checks if an inventory slot can be merged into this one.
	* @param OtherSlot	The slot we're attempting to merge into this one.
	* @return			True if OtherSlot can merge with this slot.
	*/
	UFUNCTION(BlueprintCallable, Category = "Default|Inventory")
	virtual bool CanMergeData(UInventorySlotData_Basic* OtherSlot) const;
	
	/**
	* Removes simple data from the slot.
	* @param OtherSlot	The slot to try to merge into this slot.
	* @return			True if data was merged.
	*/
	UFUNCTION(BlueprintCallable, Category = "Default|Inventory")
	virtual bool MergeData(UInventorySlotData_Basic* OtherSlot, int32& AmountOverride);

	/**
	* Check for if this slot should be destroyed.
	* Defaults to returning true - must be overwritten.
	* @return			True if the object should be destroyed.
	*/
	UFUNCTION(BlueprintCallable, Category = "Default|Inventory")
	virtual bool ShouldDestroyObject() const;

	/**
	* Attempt to find the static data for this slot.
	* @param SlotToFind			The slot pointer used to access StaticDataHandle.
	* @return					The data that was found. Can return invalid or blank data.
	*/
	UFUNCTION(BlueprintCallable, Category = "Default|Inventory")
	static FItemStaticData_Basic LookupStaticData(UInventorySlotData_Basic* SlotToFind);

	/*
	* Constructs a new UObject from the supplied data. Allows for the FItemData_Simple struct to be used to generate new slots in an inventory.
	* @param Handle		The DataTableRowHandle that is used for constructing the new UObject
	* @param NewOuter	The object that owns the new UObject for garbage collection and other functions.
	* @return			The newly-created UObject.
	*/
	UFUNCTION(BlueprintCallable, Category = "Default|Inventory")
	static UInventorySlotData_Basic* CreateNewSlotFromHandle(const FDataTableRowHandle Handle, UObject* NewOuter);
};
