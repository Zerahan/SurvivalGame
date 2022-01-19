// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventorySlotData_Basic.h"
#include "InventorySlotData_Stack.generated.h"

class ADroppedItem_Basic;

USTRUCT(BlueprintType)
struct INVENTORYSYSTEM_API FItemStaticData_Stack : public FTableRowBase {
	GENERATED_BODY()

	FItemStaticData_Stack();

	// All of the standard item static data.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemStaticData_Basic BasicData;

	// Containers aren't destroyed when they are empty. Aka, this slot remains when it's amount is 0.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsContainer;

	// The arbitrary maximum amount of this item type that can exist within a single slot.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxStackSize;

	//Amount in the slot is devided by this when displayed on UI.  This allows for stackable containers. For example, a pile of gun magazines could be 12 magazines with 20 bullets per magazine, where bullets are used from the last magazine on the stack.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StackCompressSize;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Inventory")
	//FString Subtype;

	// What to do when the item is consumed directly by the user. Used for health potions and the like.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDataAsset> AbilityClass;

	static bool IsValid(const FItemStaticData_Stack& StaticData);
};

USTRUCT(BlueprintType)
struct FItemSlotBuilder_Stack : public FItemSlotBuilder_Basic {
	GENERATED_BODY()

	FItemSlotBuilder_Stack(UDataTable* DataTable = nullptr, const FName RowName = NAME_None, const int32 Amount = -1);

	// The amount that is currently within this slot.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Inventory")
	int32 Amount;

	bool IsValid() const;
};


//*******//
// CLASS //
//*******//

/**
 * Item data that can be stacked. Ex: 20 wood, 50 stone, 40 bullets, and the like.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom))
class INVENTORYSYSTEM_API UInventorySlotData_Stack : public UInventorySlotData_Basic
{
	GENERATED_BODY()

	// The amount of this item that is in the slot
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Inventory", meta = (AllowPrivateAccess = "true"))
	int32 Amount;

public:
	UFUNCTION(BlueprintCallable, Category = "Default|Inventory")
	int32 GetAmount() const { return Amount; }

	UFUNCTION(BlueprintCallable, Category = "Default|Inventory")
	void SetAmount(const int32 NewAmount);

public:
	void SetFromData(const FItemSlotBuilder_Stack& ItemSlotBuilder);

	virtual bool AddData(const FItemData_Simple& DataToAdd, FItemData_Simple& Remainder) override;
	virtual bool RemoveData(const FItemData_Simple& DataToRemove, FItemData_Simple& Remainder) override;
	virtual bool CanMergeData(UInventorySlotData_Basic* OtherSlot) const override;
	virtual bool MergeData(UInventorySlotData_Basic* OtherSlot, int32& AmountOverride) override;
	virtual bool ShouldDestroyObject() const override;

	static FItemStaticData_Stack LookupStaticData(const UInventorySlotData_Basic* SlotToFind);
	static UInventorySlotData_Stack* CreateNewSlotFromHandle(const FDataTableRowHandle Handle, UObject* NewOuter);
};
