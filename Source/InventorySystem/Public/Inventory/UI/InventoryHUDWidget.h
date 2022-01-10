// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/MenuInterface.h"
#include "InventoryHUDWidget.generated.h"

//DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FOnInventoryHUDWidgetClosedDispatcher, UInventoryHUDWidget, OnInventoryHUDWidgetClosedDispatcher);

class UInventoryManager;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom))
class INVENTORYSYSTEM_API UInventoryHUDWidget : public UUserWidget, public IMenuInterface
{
	GENERATED_BODY()
public:
	void UpdateGraphics_Implementation() override;
	void Rebuild_Implementation() override;
	void OpenMenu_Implementation() override;
	void CloseMenu_Implementation() override;
	void ToggleMenu_Implementation() override;

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "UI|Inventory", meta=(BlueprintProtected))
	void UpdateInput();
	void UpdateInput_Implementation();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI|Inventory", meta = (BlueprintProtected, ExposeOnSpawn = "true"))
	UInventoryManager* InventoryRef;

	UPROPERTY(BlueprintReadOnly, Category = "UI|Inventory")
	bool IsOpen = false;

public:
	UFUNCTION(BlueprintCallable, Category = "UI|Inventory", meta = (BlueprintProtected))
	void SetInventoryRef(UInventoryManager* NewInventoryRef);
};
