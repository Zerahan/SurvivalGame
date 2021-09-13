// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventorySlot_Basic.h"
#include "InventorySlot_Stack.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct SURVIVALGAME_API FInventorySlotData_Stack {
	GENERATED_BODY()
	FInventorySlotData_Stack(UDataTable* DataTable = nullptr, const FName RowName = NAME_None, const int32 NewAmount = -1);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	FDataTableRowHandle StaticDataHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	int32 Amount;

	bool IsValid() const;
};

class ADroppedItem_Basic;

USTRUCT(BlueprintType)
struct SURVIVALGAME_API FItemStaticData_Stack : public FItemStaticData_Basic {
	GENERATED_BODY()

	FItemStaticData_Stack();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	int32 MaxStackSize;

	/**
	* Amount in the slot is devided by this when displayed on UI.  This allows for stackable containers. For example, a pile of gun magazines could be 12 magazines with 20 bullets per magazine, where bullets are used from the last magazine on the stack.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	int32 StackCompressSize;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	FString Subtype;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	TSubclassOf<UDataAsset> AbilityClass;

	static bool IsValid(const FItemStaticData_Stack& StaticData);
};



//*******//
// CLASS //
//*******//

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom))
class SURVIVALGAME_API UInventorySlot_Stack : public UInventorySlot_Basic
{
	GENERATED_BODY()

	int32 Amount;

	bool IsContainer;

public:
	UInventorySlot_Stack();

	UFUNCTION()
	void SetAmount(const int32 NewAmount);

	UFUNCTION()
	int32 GetAmount() const;

public:
	virtual void SetStaticDataHandle(const FDataTableRowHandle& NewStaticDataHandle) override;

	static FItemStaticData_Stack LookupStaticData(UInventorySlot_Stack* SlotToFind);

	void SetFromData(const FInventorySlotData_Stack NewSlotData);

	virtual bool AddData(const FItemData_Simple& DataToAdd, FItemData_Simple& Remainder) override;
	virtual bool RemoveData(const FItemData_Simple& DataToRemove, FItemData_Simple& Remainder) override;
	virtual bool CanMergeData(UInventorySlot_Basic* OtherSlot) const override;
	virtual bool MergeData(UInventorySlot_Basic* OtherSlot, int32& AmountOverride) override;
	virtual bool ShouldDestroyObject() const override;

	static UInventorySlot_Stack* CreateNewSlotFromHandle(const FDataTableRowHandle Handle, UObject* NewOuter);
};
