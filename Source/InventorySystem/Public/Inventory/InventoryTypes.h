// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "InventoryTypes.generated.h"

class ADroppedItem_Basic;
//class ADEPRECATED_BasicTool;
//class UDEPRECATED_BasicInventoryReserverComponent;
//class UDEPRECATED_BasicItemData;
class UInventorySlotData_Basic;
//class UDEPRECATED_BasicItemData;
//class UDEPRECATED_InventoryComponent;

UENUM(BlueprintType)
enum class EHoldingAnimation : uint8
{
	None		UMETA(DisplayName = "None"),
	Hands		UMETA(DisplayName = "Hands"),
	Carry1H		UMETA(DisplayName = "One Handed Carry"),
	Carry2H		UMETA(DisplayName = "Two Handed Carry"),
	Ranged1H	UMETA(DisplayName = "One Handed Ranged"),
	Ranged2H	UMETA(DisplayName = "Two Handed Ranged"),
	Melee1H		UMETA(DisplayName = "One Handed Melee"),
	Melee2H		UMETA(DisplayName = "Two Handed Melee"),
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None			UMETA(DisplayName = "None"),
	Item			UMETA(DisplayName = "Item"),
	Consumable		UMETA(DisplayName = "Consumable"),
	Ammo			UMETA(DisplayName = "Ammo"),
	Tool			UMETA(DisplayName = "Tool"),
	Weapon			UMETA(DisplayName = "Weapon"),
	Component		UMETA(DisplayName = "Component"),
};

UENUM(BlueprintType)
enum class EAmmoSubtype : uint8
{
	None				UMETA(DisplayName = "None"),
	Electric_10			UMETA(DisplayName = "10a Charger"),
	Electric_20			UMETA(DisplayName = "20a Charger"),
	Electric_40			UMETA(DisplayName = "40a Charger"),
	Bullet_9			UMETA(DisplayName = "9mm Round"),
	Bullet_556			UMETA(DisplayName = "5.56 Round"),
	Bullet_762			UMETA(DisplayName = "7.62 Round"),
	Bullet_Scatter		UMETA(DisplayName = "Buckshot"),
	Bullet_Slug			UMETA(DisplayName = "Slug"),
};

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EComponentSubtype : uint8
{
	None	= 0 UMETA(Hidden),
	Tool	= 1 << 0 UMETA(DisplayName = "Tool"),
	Weapon	= 1 << 1 UMETA(DisplayName = "Weapon"),
	Machine = 1 << 2 UMETA(DisplayName = "Machine"),
};
ENUM_CLASS_FLAGS(EComponentSubtype);

USTRUCT(BlueprintType)
struct INVENTORYSYSTEM_API FItemTypeRestriction {
	GENERATED_BODY()

	FItemTypeRestriction(uint8 NewTypeID = -1, bool InvertRestriction = false);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Inventory")
	uint8 TypeID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Inventory")
	bool IsInverted;

	bool IsMatching(const uint8 ItemTypeID, bool UseBitmask = false) const;
	static bool IsAllMatching(const uint8 ItemType, const uint8 ItemSubtype, const FItemTypeRestriction TypeRestriction, const FItemTypeRestriction SubtypeRestriction);
};

USTRUCT(BlueprintType)
struct INVENTORYSYSTEM_API FItemData_Simple {
	GENERATED_BODY()

	FItemData_Simple(UDataTable* DataTable = nullptr, const FName RowName = NAME_None, const int32 NewAmount = -1);
	FItemData_Simple(const uint8 NewType, const uint8 NewSubtype, const int32 NewAmount = -1);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	FDataTableRowHandle StaticDataHandle;

	uint8 Type;
	uint8 Subtype;
	//FString Subtype;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	int32 Amount;

	bool operator==(FItemData_Simple const& OtherData) const;
	bool operator!=(FItemData_Simple const& OtherData) const;
	
	//bool IsValid() const;
	static bool IsValid(const FItemData_Simple& Data);
};

/*
USTRUCT(BlueprintType)
struct INVENTORYSYSTEM_API FItemSlot {
	GENERATED_BODY()

	FItemSlot(UDataTable* DataTable = nullptr, const FName RowName = NAME_None, const int32 NewAmount = -1, UDEPRECATED_BasicItemData* NewInstancedDataRef = nullptr);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	FDataTableRowHandle StaticDataHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	float Amount;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	UDEPRECATED_BasicItemData* InstancedDataRef;
	
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
//*/


/*/
USTRUCT(BlueprintType)
struct INVENTORYSYSTEM_API FReservedInfo {
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
	UDEPRECATED_BasicInventoryReserverComponent* ReserverComponentRef;

	static bool IsValid(const FReservedInfo ReservedSlot);
};
//*/

/*/
USTRUCT(BlueprintType)
struct INVENTORYSYSTEM_API FItemStaticData : public FTableRowBase {
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
	TSubclassOf<ADEPRECATED_BasicTool> EquippedClass = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	TSubclassOf<UDEPRECATED_BasicItemData> InstancedDataClass = nullptr;
	
	//bool IsValid();

	static bool IsValid(const FItemStaticData StaticData);
};
//*/