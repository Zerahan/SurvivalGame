// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventorySlot_Basic.h"
#include "InventorySlot_Tool.generated.h"

//class ADroppedItem_Basic;
class UInventorySlot_Stack;

USTRUCT(BlueprintType, Blueprintable)
struct SURVIVALGAME_API FInventorySlotData_Tool {
	GENERATED_BODY()
	FInventorySlotData_Tool(UDataTable* DataTable = nullptr, const FName RowName = NAME_None);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	FDataTableRowHandle StaticDataHandle;

	TArray<FInventorySlotData_Stack> InternalSlots;
};

USTRUCT(BlueprintType)
struct SURVIVALGAME_API FItemStaticData_Tool : public FItemStaticData_Basic {
	GENERATED_BODY()

	FItemStaticData_Tool();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	FString AmmoSubtype;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	int32 AmmoConsumedPerUse;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	TArray<FString> InternalSlotRestrictions;

	static bool IsValid(const FItemStaticData_Tool& StaticData);
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom))
class SURVIVALGAME_API UInventorySlot_Tool : public UInventorySlot_Basic
{
	GENERATED_BODY()

	TArray<UInventorySlot_Basic*> InternalSlots;

	UInventorySlot_Stack* AmmoSlotRef;

public:
	UInventorySlot_Tool();

	void SetFromData(const FInventorySlotData_Tool NewSlotData);
	
	static FItemStaticData_Tool LookupStaticData(UInventorySlot_Tool* SlotToFind);

	virtual int32 GetAmmoCount() const;
	virtual bool ConsumeAmmo(const FItemData_Simple AmmoData);

	virtual bool AddData(const FItemData_Simple& DataToAdd, FItemData_Simple& Remainder) override;
	virtual bool RemoveData(const FItemData_Simple& DataToRemove, FItemData_Simple& Remainder) override;
	virtual bool CanMergeData(UInventorySlot_Basic* OtherSlot) const override;
	virtual bool MergeData(UInventorySlot_Basic* OtherSlot, int32& AmountOverride) override;
	virtual bool ShouldDestroyObject() const override;

	static UInventorySlot_Tool* CreateNewSlotFromHandle(const FDataTableRowHandle Handle, UObject* NewOuter);
};
