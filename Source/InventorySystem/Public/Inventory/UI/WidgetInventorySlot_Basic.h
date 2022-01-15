// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/MenuInterface.h"
#include "WidgetInventorySlot_Basic.generated.h"

class UInventoryManager;
class UInventorySlotData_Basic;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract, ClassGroup = (Custom))
class INVENTORYSYSTEM_API UWidgetInventorySlot_Basic : public UUserWidget, public IMenuInterface
{
	GENERATED_BODY()

public:
	UWidgetInventorySlot_Basic(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI|Inventory", meta = (ExposeOnSpawn = "true"))
	UInventoryManager* InventoryRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI|Inventory", meta = (ExposeOnSpawn = "true"))
	int32 TargetSlotIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI|Inventory", meta = (ExposeOnSpawn = "true"))
	UInventorySlotData_Basic* ManualSlotRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI|Inventory")
	FText DisplayName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI|Inventory")
	FText Description;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI|Inventory")
	UTexture2D* IconTexture;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI|Inventory")
	int32 Amount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI|Inventory")
	int32 CompressAmount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI|Inventory")
	int32 Health;

public:
	void UpdateGraphics_Implementation() override;
	void Rebuild_Implementation() override;
	void OpenMenu_Implementation() override;
	void CloseMenu_Implementation() override;
	void ToggleMenu_Implementation() override;

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "UI|Inventory", meta = (BlueprintProtected))
	void UpdateInput();
	virtual void UpdateInput_Implementation();

public:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintCallable, Category = "UI|Inventory")
	void SetDataFromInventory(UInventoryManager* NewInventoryRef, const int32 NewTargetSlot);

	UFUNCTION(BlueprintCallable, Category = "UI|Inventory")
	void SetDataFromSlot(UInventorySlotData_Basic* NewManualSlotRef);
};
