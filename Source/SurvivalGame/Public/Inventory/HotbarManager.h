// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryManager.h"
#include "HotbarManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_TwoParams(FOnHotbarSelectionChangedDispatcher, UHotbarManager, OnHotbarSelectionChangedDispatcher, int32, NewSelectedIndex, int32, OldSelectedIndex);

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SURVIVALGAME_API UHotbarManager : public UInventoryManager
{
	GENERATED_BODY()
private:
	int32 SelectedIndex;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default|Control|Hotbar", meta = (BlueprintProtected))
	USceneComponent* ToolAttachmentRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Control|Hotbar", meta = (BlueprintProtected))
	FName ToolAttachmentSocket;


	
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

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Default|Inventory|Control")
	FOnHotbarSelectionChangedDispatcher OnHotbarSelectionChangedDispatcher;
};
