// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "InventoryTypes.h"
#include "InventorySlot_Basic.generated.h"

class ADroppedItem_Basic;

USTRUCT(BlueprintType)
struct SURVIVALGAME_API FItemStaticData_Basic : public FTableRowBase {
	GENERATED_BODY()

	FItemStaticData_Basic();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	FString DisplayName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	FString Description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	TSoftObjectPtr<UTexture> Icon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	TSubclassOf<UInventorySlot_Basic> InstancedDataClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	TSubclassOf<ADroppedItem_Basic> SpawnedClass;

	static bool IsValid(const FItemStaticData_Basic& StaticData);
};
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract, ClassGroup = (Custom))
class SURVIVALGAME_API UInventorySlot_Basic : public UObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
	FDataTableRowHandle StaticDataHandle;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FDataTableRowHandle GetStaticDataHandle() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual void SetStaticDataHandle(const FDataTableRowHandle& NewStaticDataHandle);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static FItemStaticData_Basic LookupStaticData(UInventorySlot_Basic* SlotToFind);

	/**
	* Adds simple data to the slot.
	* @param DataToAdd	The simple data to try to add to the slot.
	* @param Remainder	Contains leftover data when the operation is completed.
	* @return			True if Remainder given anything to contain.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual bool AddData(const FItemData_Simple& DataToAdd, FItemData_Simple& Remainder);

	/**
	* Removes simple data from the slot.
	* @param DataToRemove	The simpledata to try to remove from the slot.
	* @param Remainder		Contains leftover data when the operation is completed.
	* @return				True if Remainder given anything to contain.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual bool RemoveData(const FItemData_Simple& DataToRemove, FItemData_Simple& Remainder);

	/**
	* Checks if an inventory slot can be merged into this one.
	* @param OtherSlot	The slot we're attempting to merge into this one.
	* @return			True if OtherSlot can merge with this slot.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual bool CanMergeData(UInventorySlot_Basic* OtherSlot) const;
	
	/**
	* Removes simple data from the slot.
	* @param OtherSlot	The slot to try to merge into this slot.
	* @return			True if data was merged.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual bool MergeData(UInventorySlot_Basic* OtherSlot, int32& AmountOverride);

	/**
	* Check for if this slot should be destroyed.
	* Defaults to returning true - must be overwritten.
	* @return			True if the object should be destroyed.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual bool ShouldDestroyObject() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static UInventorySlot_Basic* CreateNewSlotFromHandle(const FDataTableRowHandle Handle, UObject* NewOuter);
};
