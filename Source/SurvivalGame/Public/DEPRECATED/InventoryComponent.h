// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryTypes.h"
#include "InventoryComponent.generated.h"

class UInventoryDragDropPayload;
class UBasicInventoryReserverComponent;

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FOnInventoryChangedDispatcher, UInventoryComponent, OnInventoryChangedDispatcher);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FOnLinkedInventoryDispatcher, UInventoryComponent, OnLinkedInventoryDispatcher);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnInventorySlotChangedDispatcher, UInventoryComponent, OnInventorySlotChangedDispatcher, int32, TargetSlot);

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVALGAME_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()


public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	virtual void DestroyComponent(bool bPromoteChildren = false) override;

protected:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	/**
	* Array of slots for the inventory. Can include empty, unused slots.
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Inventory", meta = (EditFixedSize))
	TArray<FItemSlot> InventorySlots;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TArray<FReservedInfo> ReservedSlots;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	int32 LastInventorySlotIndex;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetInventorySize(const bool IncludeReservedSlots = false) const;
	/**
	* Directly overrides data within a slot in the inventory.
	* @param TargetSlot The slot index of the data to change.
	* @param ItemData The data to insert into the target slot.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (BlueprintProtected))
	void SetSlotData(const int32 TargetSlot, const FItemSlot ItemData);


public:

	/**
	* Inventory's name. Ex: Backpack
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory", meta = (ExposeOnSpawn = "true"))
	FString DisplayName;

	/**
	* Size of InventorySlots. Sets when the component is constructed.
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory", meta = (ExposeOnSpawn = "true"))
	int32 InventorySize;

	/**
	* Allows for chests or other inventories to have larger or smaller stack sizes. Only applies to stack sizes greater than 1.
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory", meta = (ExposeOnSpawn = "true"))
	float StackSizeOverrideMultiplier;

	/**
	* If set, spawns dropped items at this ref's world location
	*/
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Inventory")
	USceneComponent* SpawnPointComponentRef;

	/**
	* For handling between-inventory transfers that don't involve drag/drop
	*/
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Inventory")
	UInventoryComponent* LinkedInventoryRef;

	/**
	* If any slots are reserved, the component uses code that handles filtering out those slots
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	bool HasReservedSlots;

	/**
	* Adds reserved slots to the end of the inventory
	* @param SlotsToReserve Defines what slots to add and what component is responsible for them
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 AddReservedInfo(const FReservedInfo ReservedInfo);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool GetReservedInfo(const int32 ReservedSlotsIndex, FReservedInfo& ReservedInfo);

	/**
	* Removes reserved slots from the inventory
	* @param ReservedSlotsIndex Index within ReservedSlots that is to be removed.
	* @param ReserverRef The component that is revoking its reserved slots.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveReservedInfo(const int32 ReservedSlotsIndex, const UBasicInventoryReserverComponent* ReserverRef, const bool DropRemovedItems = false);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UBasicInventoryReserverComponent* GetReserveComponent(const int32 TargetSlot) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetLinkedInventoryRef(UInventoryComponent* NewLinkedInventoryRef);

	/**
	* Returns owning actor's world location, otherwise returns SpawnPointComponentRef's world location
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FVector GetWorldSpawnLocation();

	/**
	* Returns owning actor's world rotation, otherwise returns SpawnPointComponentRef's world rotation
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FRotator GetWorldSpawnRotation();

	/**
	* Gets max stack size from StaticData, altered by StackSizeOverrideMultiplier if MaxStackSize is greater than 1.
	* @param StaticData Static Data reference to pull MaxStackSize from
	* @return MaxStackSize
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	float GetStackSizeWithOverride(const FItemStaticData StaticData) const;

	/**
	* Gets a copy of the target slot from the inventory.
	* @param TargetSlot The slot index to copy data from.
	* @param ReturnData A copy of the data from the TargetSlot of the inventory.
	* @return True if TargetSlot is a valid index.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool GetSlotData(const int32 TargetSlot, FItemSlot& ReturnData) const;

	/**
	* Checks if the provided ItemData is valid.
	* @param ItemData Item data used for the check
	* @return Returns true if ItemData is valid.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static bool IsItemSlotValid(const FItemSlot& ItemData);

	/**
	* Gets static data for the item type from the data tables.
	* @param ItemData Item data used for the search
	* @param ReturnData The found static data.
	* @return Returns true if valid static data was found.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static bool LookupStaticData(const FItemSlot& ItemData, FItemStaticData& ReturnData);



	//-----------------------//
	// Information Gathering //
	//-----------------------//



	/**
	* Checks if the target slot exists.
	* @param TargetSlot Index of the slot to check.
	* @return True if the TargetSlot is a valid index in the inventory array.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool IsSlotValid(const int32 TargetSlot) const;

	/**
	* Checks if the target slot contains nothing.
	* @param TargetSlot Index of the slot to check.
	* @return True if the data at the TargetSlot index is invalid, aka empty.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool IsSlotEmpty(const int32 TargetSlot) const;

	/**
	* Checks if the target slot matches the type
	* @param TargetSlot Index of the slot to check.
	* @param ItemData Data to compare the slot to.
	* @return True if the target slot's type data matches the input item data's type.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool IsSlotOfType(const int32 TargetSlot, const FItemSlot& ItemData) const;

	/**
	* Checks if TargetSlot is reserved by checking the ReservedSlots array for valid component references.
	* @param TargetSlot Index of the slot to check.
	* @return True if the TargetSlot in ReservedSlots is valid
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool IsSlotReserved(const int32 TargetSlot) const;

	/**
	* Gets a list of indicies for InventorySlots
	* @param IncludeReservedSlots If false, ignores slots with valid ReservedSlots pairings
	* @return Returns an array of InventorySlots indicies
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<int32> GetInventory(const bool IncludeReservedSlots = false) const;

	/**
	* Get how much of an item type is in the inventory.
	* @param ItemData Item data used for the search
	* @return How much of the ItemData type is in the inventory.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	float GetTotalAmountOfType(const FItemSlot& ItemData) const;

	/**
	* Get how much of an item type is in the inventory. Also returns the matching slot indicies.
	* @param ItemData Item data used for the search
	* @param TargetSlots Slot indicies of all slots whos type matches ItemData. Will add new data to end of array.
	* @return How much of the ItemData type is in the inventory.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	float GetTotalAmountOfTypeAndSlots(const FItemSlot& ItemData, TArray<int32>& TargetSlots) const;

	/**
	* Finds indicies of all empty slots within the inventory.
	* @param TargetSlots Array of indicies pointing to empty slots. Will add new data to end of array.
	* @return True if any empty slots were found.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool FindAllEmptySlots(TArray<int32>& TargetSlots) const;

	/**
	* Finds indicies of all matching slots within the inventory.
	* @param ItemData Slot data to look for.
	* @param TargetSlots Array of indicies pointing to matching slots. Will add new data to end of array.
	* @return True if any matching slots were found.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool FindAllSlotsOfType(const FItemSlot& ItemData, TArray<int32>& TargetSlots) const;



	//---------------------------//
	// Manipulation Verification //
	//---------------------------//



	/**
	* Gets if ItemData can be fully added to inventory
	* @param ItemData Item data used for the search
	* @return Can add ItemData to inventory.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool CanAddToInventory(const FItemSlot& ItemData) const;

	/**
	* Checks if the amount in ItemData can be removed from the inventory
	* @param ItemData Type of item and amount to check.
	* @return Does inventory contain at least ItemData.Amount of ItemData's type?
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool CanRemoveFromInventory(const FItemSlot& ItemData);



	//--------------//
	// Manipulation //
	//--------------//



	/**
	* Adds data to a specific slot in the inventory.
	* @param TargetSlot The slot you are looking to add data to.
	* @param ItemData The data you want to add to the target slot.
	* @param Remainder How much is left in ItemData from the operation. Ex: trying to add 5 units to a slot that only has room for 4 units will result in 1 unit remaining.
	* @return Has Remainder. True if the operation leaves a remainder, false otherwise.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddToSlot(const int32 TargetSlot, const FItemSlot& ItemData, FItemSlot& Remainder);

	/**
	* Removes data from a specific slot in the inventory.
	* @param TargetSlot The slot you are looking to remove data from.
	* @param ItemData The data you want to remove from the target slot.
	* @param Remainder How much is left in ItemData from the operation. Ex: trying to remove 5 units from a slot that has only 4 units will result in 1 unit remaining.
	* @return Has Remainder. True if the operation leaves a remainder, false otherwise.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveFromSlot(const int32 TargetSlot, const FItemSlot& ItemData, FItemSlot& Remainder);

	/**
	* Attempt to add all of ItemData to inventory by looping through all of its slots.
	* @param ItemData The type of data and the amount to add.
	* @param Remainder How much was left over from the operation.
	* @return True if there was any remainder from the operation.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddToInventory(const FItemSlot& ItemData, FItemSlot& Remainder);

	/**
	* Attempt to remove all of ItemData's amount from inventory by looping through all of its slots.
	* @param ItemData The type of data and the amount to remove.
	* @param Remainder How much was left over from the operation.
	* @return True if there was any remainder from the operation.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveFromInventory(const FItemSlot& ItemData, FItemSlot& Remainder);

	/**
	* Transfer data from one inventory to this inventory.
	* @param ToSlot Slot index to add data to.
	* @param FromComponent Component to remove data from.
	* @param FromSlot Slot index to remove data from.
	* @param Amount Amount to try to move from FromSlot
	* @return True if data was transfered.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool TransferToSlot(const int32 ToSlot, UInventoryComponent* FromComponent, const int32 FromSlot, const float Amount = -1.f);

	/**
	* Transfer data from one inventory to any available space in this inventory.
	* @param FromComponent Component to remove data from.
	* @param FromSlot Slot index to remove data from.
	* @param Amount Amount to try to move from FromSlot. If Amount is -1, try to transfer everything in FromSlot. If Amount is -2, try to transfer everything in FromComponent of the type in FromSlot.
	* @return True if data was transfered.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool TransferToInventory(UInventoryComponent* FromComponent, const int32 FromSlot, const float Amount = -1.f);
	
	/**
	* Swap data between two slots.
	* @param ToSlot Slot index to swap data to.
	* @param FromComponent Inventory to swap data from.
	* @param FromSlot Slot index to swap data from.
	* @return True if data was swapped.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool TransferSwapSlots(const int32 ToSlot, UInventoryComponent* FromComponent, const int32 FromSlot);

	/**
	* Attempts to add FromSlot in FromInventoryRef to ToSlot in ToInventoryRef.
	* If ToInventoryRef is invalid, attempts to spawn the item in FromSlot, while deleting it from the inventory.
	* If ToSlot is -1, attempts to move all of FromSlot into ToInventory.
	* If the data in FromSlot and ToSlot are the same, or if ToSlot is empty, attempts to merge the data.
	* If the data in FromSlot and ToSlot are different, attempts to swap the data between them.
	* @param Payload Drag/Drop payload.
	* @param CallingController The controller that started the operation.
	* @return True if data was swapped.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool DragDropHandler(const UInventoryDragDropPayload* Payload, const APawn* CallingPawn);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	ADroppedItem_Basic* CreateDroppedItem(const FItemStaticData& StaticData);

//	UFUNCTION(BlueprintCallable, Category = "Inventory")
//	bool TransferHandler(UInventoryComponent* FromComponent, const int32 FromSlot, const float Amount = -1.f);



	//-------------//
	// Dispatchers //
	//-------------//



	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Inventory")
	FOnInventoryChangedDispatcher OnInventoryChangedDispatcher;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Inventory")
	FOnInventorySlotChangedDispatcher OnInventorySlotChangedDispatcher;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Inventory")
	FOnLinkedInventoryDispatcher OnLinkedInventoryDispatcher;
};
