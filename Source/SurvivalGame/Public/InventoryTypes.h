// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "InventoryTypes.generated.h"

class ADroppedItem_Basic;
class ABasicTool;
class UBasicInventoryReserverComponent;
class UBasicItemData;
class UInventorySlot_Basic;
class UBasicItemData;
//class UInventoryComponent;

UENUM(BlueprintType)
enum EItemSubtype
{
	None				UMETA(DisplayName = "None"),
	Item				UMETA(DisplayName = "Item"),
	Ammo				UMETA(DisplayName = "Ammo"),
	Ammo_Electricity	UMETA(DisplayName = "Ammo|Electricity"),
	Ammo_Bullet			UMETA(DisplayName = "Ammo|Bullet"),
	Tool				UMETA(DisplayName = "Tool"),
	Weapon				UMETA(DisplayName = "Weapon"),
	ToolPart			UMETA(DisplayName = "Component"),
	ToolPart_Welder		UMETA(DisplayName = "Component|Welder"),
};

USTRUCT(BlueprintType)
struct SURVIVALGAME_API FItemData_Simple {
	GENERATED_BODY()

	//FItemData_Simple() : FItemData_Simple(nullptr, NAME_None, -1, -1) {}
	//FItemData_Simple(UDataTable* DataTable, const FName RowName) : FItemData_Simple(DataTable, RowName, -1, -1) {}
	//FItemData_Simple(UDataTable* DataTable, const FName RowName, const int32 NewAmount = -1) : FItemData_Simple(DataTable, RowName, NewAmount, -1) {}
	FItemData_Simple(UDataTable* DataTable = nullptr, const FName RowName = NAME_None, const int32 NewAmount = -1, const FString NewSubtype = "None");
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	FDataTableRowHandle StaticDataHandle;

	FString Subtype;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	int32 Amount;

	bool operator==(FItemData_Simple const& OtherData) const;
	bool operator!=(FItemData_Simple const& OtherData) const;
	
	//bool IsValid() const;
	static bool IsValid(const FItemData_Simple& Data);
};

USTRUCT(BlueprintType)
struct SURVIVALGAME_API FItemSlot {
	GENERATED_BODY()

	FItemSlot(UDataTable* DataTable = nullptr, const FName RowName = NAME_None, const int32 NewAmount = -1, UBasicItemData* NewInstancedDataRef = nullptr);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	FDataTableRowHandle StaticDataHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	float Amount;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	UBasicItemData* InstancedDataRef;
	
	//bool IsValid();

	static bool IsValid(const FItemSlot ItemData);
	
	bool operator==(const FItemSlot ItemData) {
		return StaticDataHandle.RowName == ItemData.StaticDataHandle.RowName;//&& StaticDataHandle.DataTable == ItemData.StaticDataHandle.DataTable;// && ((InstancedDataRef && ItemData.InstancedDataRef && InstancedDataRef == ItemData.InstancedDataRef) || (!InstancedDataRef && !ItemData.InstancedDataRef));
	}
	
	bool operator!=(const FItemSlot ItemData) {
		return !(*this == ItemData);
	}

	static FString ToString(const FItemSlot ItemData){
		if (IsValid(ItemData)) {
			return "Item: " + ItemData.StaticDataHandle.RowName.ToString() + " @ " + ItemData.StaticDataHandle.DataTable->GetFName().ToString() + " #" + FString::SanitizeFloat(ItemData.Amount);
		}
		else {
			return "Invalid ItemData";
		}
	}
};


USTRUCT(BlueprintType)
struct SURVIVALGAME_API FReservedInfo {
	GENERATED_BODY()

	FReservedInfo()
		: StartIndex(-1)
		, NumSlots(-1)
	{

	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	int32 StartIndex;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	int32 NumSlots;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	UBasicInventoryReserverComponent* ReserverComponentRef;

	static bool IsValid(const FReservedInfo ReservedSlot);
};

USTRUCT(BlueprintType)
struct SURVIVALGAME_API FItemStaticData : public FTableRowBase {
	GENERATED_BODY()

	FItemStaticData()
		: DisplayName("DEFAULT_NAME")
		, Description("DEFAULT_DESC")
		, MaxStackSize(1)
		, AmountIsInteger(true)
	{}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	FString DisplayName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	FString Description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	int32 MaxStackSize;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	bool AmountIsInteger;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	TSubclassOf<ADroppedItem_Basic> SpawnedClass = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	TSubclassOf<ABasicTool> EquippedClass = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	TSubclassOf<UBasicItemData> InstancedDataClass = nullptr;
	
	//bool IsValid();

	static bool IsValid(const FItemStaticData StaticData);
};