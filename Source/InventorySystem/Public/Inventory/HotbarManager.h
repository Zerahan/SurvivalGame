// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryManager.h"
#include "HotbarManager.generated.h"

class AItemTool_Basic;
class UInventorySlotData_Tool;

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_TwoParams(FOnHotbarSelectionChangedDispatcher, UHotbarManager, OnHotbarSelectionChangedDispatcher, int32, NewSelectedIndex, int32, OldSelectedIndex);

/**
 * A type of InventoryManager that allows for actors spawned in as usable equipment, and the ability to have specific slots selected for that purpose.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UHotbarManager : public UInventoryManager
{
	GENERATED_BODY()
private:
	// The currently-selected slot index.
	int32 SelectedIndex;

	// The slot containing the default equipment that is used when no valid slot is selected.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default|Control|Hotbar", meta = (AllowPrivateAccess = "true"))
	UInventorySlotData_Tool* HandSlotData;

public:
	UHotbarManager();

protected:
	// The component that spawned tools should be attached to.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default|Control|Hotbar", meta = (BlueprintProtected))
	USceneComponent* ToolAttachmentRef;

	// The component socket that spawned tools should be attached to.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Control|Hotbar", meta = (BlueprintProtected))
	FName ToolAttachmentSocket;

	// All of the currently-spawned tools.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Control|Hotbar", meta = (BlueprintProtected))
	TArray<AItemTool_Basic*> SpawnedEquipment;

	// The spawned tool that is used when no valid slot is selected.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Control|Hotbar", meta = (BlueprintProtected))
	AItemTool_Basic* HandEquipment;

	// The static data used to generate the default tools's slot and spawned tool. Only used on the editor-side for set up.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default|Control|Hotbar", meta = (BlueprintProtected))
	FDataTableRowHandle HandStaticData;

	virtual void BeginPlay() override;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Default|Control|Hotbar")
	USceneComponent* GetToolAttachmentComponent() const;

	UFUNCTION(BlueprintCallable, Category = "Default|Control|Hotbar")
	FName GetToolAttachmentSocket() const;

	UFUNCTION(BlueprintCallable, Category = "Default|Control|Hotbar")
	void SetToolAttachment(USceneComponent* Component = nullptr, FName Socket = NAME_None);

	UFUNCTION(BlueprintCallable, Category = "Default|Inventory|Control")
	int32 GetSelectedIndex() const;

	UFUNCTION(BlueprintCallable, Category = "Default|Inventory|Control")
	void SetSelectedIndex(const int32 NewIndex);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default|Inventory|Control")
	void SelectItem(const int32 ItemIndex = 0);
	virtual void SelectItem_Implementation(const int32 ItemIndex = 0);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default|Inventory|Control")
	void SelectNextItem();
	virtual void SelectNextItem_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default|Inventory|Control")
	void SelectPreviousItem();
	virtual void SelectPreviousItem_Implementation();

	virtual void OnInventoryManagerSlotChanged_Implementation(const int32 TargetSlot) override;

	/*
	* Handler for when a new slot is selected.
	* @param NewSelectedIndex	The newly selected index.
	* @param OldSelectedIndex	The newly unselected index.
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Default|Inventory")
	void OnHotbarSelectionChanged(const int32 NewSelectedIndex, const int32 OldSelectedIndex);
	virtual void OnHotbarSelectionChanged_Implementation(const int32 NewSelectedIndex, const int32 OldSelectedIndex);

	/*
	* The event dispatcher called when a slot is selected.
	* @param NewSelectedIndex	The newly selected index.
	* @param OldSelectedIndex	The newly unselected index.
	*/
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Default|Inventory|Control")
	FOnHotbarSelectionChangedDispatcher OnHotbarSelectionChangedDispatcher;
};
