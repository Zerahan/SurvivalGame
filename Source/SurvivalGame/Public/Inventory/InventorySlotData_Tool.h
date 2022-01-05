// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventorySlotData_Basic.h"
#include "Inventory/InventorySlotData_Stack.h"
#include "InventorySlotData_Tool.generated.h"

//class ADroppedItem_Basic;
//class UInventorySlotData_Stack;
class AItemTool_Basic;

USTRUCT(BlueprintType, Blueprintable)
struct SURVIVALGAME_API FItemSlotBuilder_Tool : public FItemSlotBuilder_Basic {
	GENERATED_BODY()
	FItemSlotBuilder_Tool(UDataTable* DataTable = nullptr, const FName RowName = NAME_None);

	TArray<FItemSlotBuilder_Stack> InternalSlots;

	bool IsValid() const;
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	TSubclassOf<AItemTool_Basic> EquippedClass;

	static bool IsValid(const FItemStaticData_Tool& StaticData);
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom))
class SURVIVALGAME_API UInventorySlotData_Tool : public UInventorySlotData_Basic
{
	GENERATED_BODY()
	TArray<UInventorySlotData_Basic*> InternalSlots;

	UInventorySlotData_Stack* AmmoSlotRef;

public:
	UInventorySlotData_Tool();

	void SetFromData(const FItemSlotBuilder_Tool& NewSlotData);

	virtual int32 GetAmmoCount() const;
	virtual bool ConsumeAmmo(const FItemData_Simple AmmoData);

	virtual bool AddData(const FItemData_Simple& DataToAdd, FItemData_Simple& Remainder) override;
	virtual bool RemoveData(const FItemData_Simple& DataToRemove, FItemData_Simple& Remainder) override;
	virtual bool CanMergeData(UInventorySlotData_Basic* OtherSlot) const override;
	virtual bool MergeData(UInventorySlotData_Basic* OtherSlot, int32& AmountOverride) override;
	virtual bool ShouldDestroyObject() const override;

	static FItemStaticData_Tool LookupStaticData(UInventorySlotData_Basic* SlotToFind);
	static UInventorySlotData_Tool* CreateNewSlotFromHandle(const FDataTableRowHandle Handle, UObject* NewOuter);
};
