// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Inventory/InventoryTypes.h"
#include "InventorySlotData_Basic.generated.h"

class ADroppedItem_Basic;

// Serialize with FObjectWriter/FObjectReader

USTRUCT(BlueprintType)
struct INVENTORYSYSTEM_API FItemStaticData_Basic : public FTableRowBase {
	GENERATED_BODY()

	FItemStaticData_Basic();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 ItemSubtype;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UInventorySlotData_Basic> InstancedDataClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ADroppedItem_Basic> SpawnedClass;

	static bool IsValid(const FItemStaticData_Basic& StaticData);
};

USTRUCT(BlueprintType)
struct FItemSlotBuilder_Basic {
	GENERATED_BODY()

public:
	FItemSlotBuilder_Basic(UDataTable* DataTable = nullptr, const FName RowName = NAME_None);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Inventory")
	FDataTableRowHandle StaticDataHandle;

	bool IsValid() const;
};
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract, ClassGroup = (Custom))
class INVENTORYSYSTEM_API UInventorySlotData_Basic : public UObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default|Inventory", meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	FDataTableRowHandle StaticDataHandle;

public:
	void SetFromData(const FItemSlotBuilder_Basic& NewSlotData);

	UFUNCTION(BlueprintCallable)
	FDataTableRowHandle GetStaticDataHandle() const;

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
	* Attempt to find the static data for this handle.
	* @param StaticDataHandle	The handle to use for looking up the static data
	* @param ReturnedStaticData	The actual data that was found.
	* @return					True if the handle pointed to valid static data.
	*/
	UFUNCTION(BlueprintCallable, Category = "Default|Inventory")
	static FItemStaticData_Basic LookupStaticData(UInventorySlotData_Basic* SlotToFind);

	UFUNCTION(BlueprintCallable, Category = "Default|Inventory")
	static UInventorySlotData_Basic* CreateNewSlotFromHandle(const FDataTableRowHandle Handle, UObject* NewOuter);
};
