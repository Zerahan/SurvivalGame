// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryManager.generated.h"

class UInventorySlot_Basic;
class UInventoryManager_DragPayload;
class ADroppedItem_Basic;
struct FItemStaticData_Basic;
struct FDataTableRowHandle;

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnInventoryManagerSlotChangedDispatcher, UInventoryManager, OnInventoryManagerSlotChangedDispatcher, int32, TargetSlot);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FOnLinkedInventoryChangedDispatcher, UInventoryManager, OnLinkedInventoryChangedDispatcher);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURVIVALGAME_API UInventoryManager : public UActorComponent
{
	GENERATED_BODY()
		
private:
	/**
	* Inventory's name. Ex: Backpack
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory", meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	FString DisplayName;

	/**
	* Size of InventorySlots. Sets size of InventorySlots on BeginPlay
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory", meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	int32 InventorySize;

	/**
	* Array of slots for the inventory. Can include empty, unused slots.
	*/
	UPROPERTY(VisibleAnywhere, Category = "Inventory", meta = (EditFixedSize))
	TArray<UInventorySlot_Basic*> InventorySlots;

	/**
	* False: Only the classes in AllowedItemTypes are allowed to be added to this inventory
	* True: Everything but the classes in AllowedItemTypes are allowed to be added to this inventory
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory", meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	bool BanOnlyAllowedItemTypes;
	
	/**
	* List of classes allowed into the slots. Can be inverted with BanOnlyAllowedItemTypes.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory", meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UInventorySlot_Basic>> AllowedItemTypes;

	/**
	* Allows transfers between this inventory and another. Specifically useful for chests and other storage mediums seperate from the player. Should only allow a server to set this variable.
	*/
	UPROPERTY()
	UInventoryManager* LinkedInventoryRef;

	/**
	* Does the actual operation for AddToSlot. Assumes incoming data is valid.
	*/
	bool AddToSlot_Internal(const int32 TargetSlot, const FItemData_Simple& SimpleData, FItemData_Simple& Remainder);

	/**
	* Does the actual operation for RemoveFromSlot. Assumes incoming data is valid.
	*/
	bool RemoveFromSlot_Internal(const int32 TargetSlot, const FItemData_Simple& SimpleData, FItemData_Simple& Remainder);

public:	
	// Sets default values for this component's properties
	UInventoryManager();
	
	/**
	* If set, spawns dropped items at this ref's world location
	*/
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Inventory")
	USceneComponent* SpawnPointComponentRef;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetInventorySize(const int32 NewSize);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetDisplayName(const FString NewName);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FString GetDisplayName() const;

	/**
	* Sets the seperate inventory that this inventory can make transfers with. Example: link the player's inventory and a chests' inventory to allow transfers. Should only allow the server to use this.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetLinkedInventory(UInventoryManager* NewLinkedInventoryRef = nullptr);

	/**
	* Gets the inventory linked to this one.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UInventoryManager* GetLinkedInventory() const;

	/**
	* Returns owning actor's world location, otherwise returns SpawnPointComponentRef's world location
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FVector GetWorldSpawnLocation() const;

	/**
	* Returns owning actor's world rotation, otherwise returns SpawnPointComponentRef's world rotation
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FRotator GetWorldSpawnRotation() const;

	/**
	* Gets the size of the inventory.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetInventorySize() const;

	/**
	* Gets a reference to the target slot in the inventory.
	* @param TargetSlot	The slot index to copy data from.
	* @return			The slot's pointer.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UInventorySlot_Basic* GetSlot(const int32 TargetSlot) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetIndex(UInventorySlot_Basic* SlotRef) const;

	/**
	* Directly sets data within a slot in the inventory.
	* @param TargetSlot				The slot index of the data to change.
	* @param NewInventorySlotRef	The data to insert into the target slot.
	* @param DoOverride				True if the function is allowed to overwrite existing data.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetSlot(const int32 TargetSlot, UInventorySlot_Basic* NewInventorySlotRef, const bool DoOverride = false);

	/**
	* Checks if the target slot contains nothing.
	* @param TargetSlot	Index of the slot to check.
	* @return			True if the data at the TargetSlot index is invalid, aka empty.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool IsSlotEmpty(const int32 TargetSlot) const;

	/**
	* Checks if the target slot contains a match.
	* @param TargetSlot	Index of the slot to check.
	* @return			True if the data at the TargetSlot index is invalid, aka empty.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool IsSlotOfType(const int32 TargetSlot, const FDataTableRowHandle StaticDataHandle) const;

	/**
	* Checks if the inventory contains at least the amount given of a particular type. Optional: Also return the slots that contained the amount.
	* @param AmountToFind		The amount to check for in the inventory.
	* @param StaticDataHandle	The item type to search for.
	* @param TargetSlots		The slots that contained the found amount.
	* @return					True if the inventory contains at least AmountToFind items of type StaticDataHandle.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory", meta=(AutoCreateRefTerm="TargetSlots"))
	bool ContainsAtLeast(const int32 AmountToFind, const FDataTableRowHandle StaticDataHandle, TArray<int32>& TargetSlots) const;

	/*
	* Finds the previous slot that matches the item type given. Starts at TargetSlot and continues until a match is found or the end of the array is reached.
	* @param TargetSlot			The current index to check. Returns the index of the next matching slot.
	* @param StaticDataHandle	The item type to find. Can also be used to find empty slots.
	* @return					True if a match was found. False if the end of the array was reached without a match found.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool NextSlotOfType(int32& TargetSlot, const FDataTableRowHandle& StaticDataHandle);

	/*
	* Finds the previous slot that matches the item type given. Starts at TargetSlot and continues until a match is found or the start of the array is reached.
	* @param TargetSlot			The current index to check. Returns the index of the next matching slot.
	* @param StaticDataHandle	The item type to find. Can also be used to find empty slots.
	* @return					True if a match was found. False if the end of the array was reached without a match found.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool PreviousSlotOfType(int32& TargetSlot, const FDataTableRowHandle& StaticDataHandle);

	/**
	* Finds indicies of all matching slots within the inventory.
	* @param SimpleData		Slot data to look for.
	* @param TargetSlots	Array of indicies pointing to matching slots. Will add new data to end of array.
	* @return				True if any matching slots were found.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	void FindSlotsOfType(const FDataTableRowHandle& StaticDataHandle, TArray<int32>& TargetSlots) const;

	/**
	* Finds indicies of all empty slots within the inventory.
	* @param TargetSlots	Array of indicies pointing to empty slots. Will add new data to end of array.
	* @return				True if any empty slots were found.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	void FindEmptySlots(TArray<int32>& TargetSlots) const;

	/**
	* Adds simple data to the slot. Creates a new slot object if there was none at the TargetSlot index.
	* @param TargetSlot	The index of the slot to try to add data to.
	* @param SimpleData	The simple data to try to add to the slot.
	* @param Remainder	Contains any leftover data when the operation is completed.
	* @return			True if Remainder given anything to contain.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddToSlot(const int32 TargetSlot, const FItemData_Simple& SimpleData, FItemData_Simple& Remainder);

	/**
	* Removes simple data from the slot
	* @param TargetSlot	The index of the slot to try to remove data from.
	* @param SimpleData	The simple data to try to remove from the slot.
	* @param Remainder	Contains leftover data when the operation is completed.
	* @return			True if Remainder given anything to contain.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveFromSlot(const int32 TargetSlot, const FItemData_Simple& SimpleData, FItemData_Simple& Remainder);

	bool MergeToSlot(const int32 TargetSlot, UInventorySlot_Basic* OtherSlotRef);
	/**
	* Merges the data of two slots
	* @param TargetSlot		The index of the slot to try to merge into.
	* @param OtherSlotRef	The slot reference to merge.
	* @return				True if data was merged.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool MergeToSlot(const int32 TargetSlot, UInventorySlot_Basic* OtherSlotRef, int32& Remainder);

	/**
	* Attempt to add all of ItemData to inventory by looping through all of its slots.
	* @param SimpleData	The type of data and the amount to add.
	* @param Remainder	How much was left over from the operation.
	* @return			True if there was any remainder from the operation.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddToInventory(const FItemData_Simple& SimpleData, FItemData_Simple& Remainder);

	/**
	* Attempt to remove all of ItemData's amount from inventory by looping through all of its slots.
	* @param SimpleData	The type of data and the amount to remove.
	* @param Remainder	How much was left over from the operation.
	* @return			True if there was any remainder from the operation.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveFromInventory(const FItemData_Simple& SimpleData, FItemData_Simple& Remainder);

	/**
	* Adds or merges the data into the inventory
	* @param OtherSlotRef	The slot reference to merge.
	* @return				True if data was merged.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool MergeToInventory(UInventorySlot_Basic* OtherSlotRef, int32& Remainder);

	/**
	* Transfer data from one slot to another slot. Can transfer between two inventories, or within one inventory.
	* @param ToSlot				Slot index to add data to.
	* @param FromInventoryRef	Inventory to remove data from.
	* @param FromSlot			Slot index to remove data from.
	* @param Amount				Amount to try to move from FromSlot
	* @return					True if data was transfered.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool TransferToSlot(const int32 ToSlot, UInventoryManager* FromInventoryRef, const int32 FromSlot, const float AmountOverride = -1.f);

	/**
	* Transfer data from a slot to any available slot. Can transfer between two inventories, or within one inventory.
	* @param FromInventoryRef	Inventory to remove data from.
	* @param FromSlot			Slot index to remove data from.
	* @param Amount				Amount to try to move from FromSlot. If Amount is -1, try to transfer everything in FromSlot. If Amount is -2, try to transfer everything in FromComponent of the type in FromSlot.
	* @return					True if data was transfered.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool TransferToInventory(UInventoryManager* FromInventoryRef, const int32 FromSlot, const float AmountOverride = -1.f);

	/**
	* Swap data between two slots. Can transfer between two inventories, or within one inventory.
	* @param ToSlot				Slot index to swap data to.
	* @param FromInventoryRef	Inventory to swap data from.
	* @param FromSlot			Slot index to swap data from.
	* @return					True if data was swapped.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool TransferSwapSlots(const int32 ToSlot, UInventoryManager* FromInventoryRef, const int32 FromSlot);

	/**
	* Attempts to add FromSlot in FromInventoryRef to ToSlot in ToInventoryRef.
	* If ToInventoryRef is invalid, attempts to spawn the item in FromSlot, while deleting it from the inventory.
	* If ToSlot is -1, attempts to move all of FromSlot into ToInventory.
	* If the data in FromSlot and ToSlot are the same, or if ToSlot is empty, attempts to merge the data.
	* If the data in FromSlot and ToSlot are different, attempts to swap the data between them.
	* @param Payload Drag/Drop payload.
	* @param CallingController The controller that started the operation.
	* @return True if data was handled.
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool HandleDragDropPayload(const UInventoryManager_DragPayload* Payload, const APawn* CallingPawn);


	/**
	* Helper function for spawning items into the world from an inventory
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	ADroppedItem_Basic* CreateDroppedItem(const TSubclassOf<ADroppedItem_Basic> SpawnedClass);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Inventory")
	FOnInventoryManagerSlotChangedDispatcher OnInventoryManagerSlotChangedDispatcher;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Inventory")
	FOnLinkedInventoryChangedDispatcher OnLinkedInventoryChangedDispatcher;
};
