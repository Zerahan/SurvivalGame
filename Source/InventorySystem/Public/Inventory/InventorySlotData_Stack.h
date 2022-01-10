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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemStaticData_Basic BasicData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxStackSize;

	/**
	* Amount in the slot is devided by this when displayed on UI.  This allows for stackable containers. For example, a pile of gun magazines could be 12 magazines with 20 bullets per magazine, where bullets are used from the last magazine on the stack.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StackCompressSize;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Inventory")
	//FString Subtype;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDataAsset> AbilityClass;

	static bool IsValid(const FItemStaticData_Stack& StaticData);
};

USTRUCT(BlueprintType)
struct FItemSlotBuilder_Stack : public FItemSlotBuilder_Basic {
	GENERATED_BODY()

	FItemSlotBuilder_Stack(UDataTable* DataTable = nullptr, const FName RowName = NAME_None, const int32 Amount = -1);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Inventory")
	int32 Amount;

	bool IsValid() const;
};


//*******//
// CLASS //
//*******//

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom))
class INVENTORYSYSTEM_API UInventorySlotData_Stack : public UInventorySlotData_Basic
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
