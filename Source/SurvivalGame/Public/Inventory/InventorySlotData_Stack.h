// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventorySlotData_Basic.h"
#include "InventorySlotData_Stack.generated.h"

class ADroppedItem_Basic;

USTRUCT(BlueprintType)
struct FItemSlotBuilder_Stack : public FItemSlotBuilder_Basic {
	GENERATED_BODY()

	FItemSlotBuilder_Stack(UDataTable* DataTable = nullptr, const FName RowName = NAME_None, const int32 Amount = -1);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Inventory")
	int32 Amount;

	bool IsValid() const;
};

USTRUCT(BlueprintType)
struct SURVIVALGAME_API FItemStaticData_Stack : public FItemStaticData_Basic {
	GENERATED_BODY()

	FItemStaticData_Stack();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Inventory")
	bool IsContainer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Inventory")
	uint8 TypeID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Inventory")
	uint8 SubTypeID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Inventory")
	int32 MaxStackSize;

	/**
	* Amount in the slot is devided by this when displayed on UI.  This allows for stackable containers. For example, a pile of gun magazines could be 12 magazines with 20 bullets per magazine, where bullets are used from the last magazine on the stack.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Inventory")
	int32 StackCompressSize;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Inventory")
	FString Subtype;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Inventory")
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
class SURVIVALGAME_API UInventorySlotData_Stack : public UInventorySlotData_Basic
{
	GENERATED_BODY()

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
