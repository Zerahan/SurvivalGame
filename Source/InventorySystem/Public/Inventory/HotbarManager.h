// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryManager.h"
#include "HotbarManager.generated.h"

class AItemTool_Basic;
class UInventorySlotData_Tool;

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_TwoParams(FOnHotbarSelectionChangedDispatcher, UHotbarManager, OnHotbarSelectionChangedDispatcher, int32, NewSelectedIndex, int32, OldSelectedIndex);

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UHotbarManager : public UInventoryManager
{
	GENERATED_BODY()
private:
	int32 SelectedIndex;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default|Control|Hotbar", meta = (AllowPrivateAccess = "true"))
	UInventorySlotData_Tool* HandSlotData;

public:
	UHotbarManager();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default|Control|Hotbar", meta = (BlueprintProtected))
	USceneComponent* ToolAttachmentRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Control|Hotbar", meta = (BlueprintProtected))
	FName ToolAttachmentSocket;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Control|Hotbar", meta = (BlueprintProtected))
	TArray<AItemTool_Basic*> SpawnedEquipment;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Control|Hotbar", meta = (BlueprintProtected))
	AItemTool_Basic* HandEquipment;

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

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Default|Inventory")
	void OnHotbarSelectionChanged(const int32 NewSelectedIndex, const int32 OldSelectedIndex);
	virtual void OnHotbarSelectionChanged_Implementation(const int32 NewSelectedIndex, const int32 OldSelectedIndex);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Default|Inventory|Control")
	FOnHotbarSelectionChangedDispatcher OnHotbarSelectionChangedDispatcher;
};
