// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryTypes.h"
#include "BasicItemSlotWidget.generated.h"

class UInventoryComponent;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom))
class SURVIVALGAME_API UBasicItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI|Inventory", meta = (ExposeOnSpawn = "true"))
	UInventoryComponent* InventoryRef = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI|Inventory", meta = (ExposeOnSpawn = "true"))
	int32 TargetSlotIndex = -1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI|Inventory", meta = (ExposeOnSpawn = "true"))
	FItemSlot ItemData = FItemSlot();

public:
	UFUNCTION(BlueprintCallable, Category = "UI|Inventory", meta = (BlueprintProtected))
	void SetInventoryRef(UInventoryComponent* NewInventoryRef);

	UFUNCTION(BlueprintCallable, Category = "UI|Inventory", meta = (BlueprintProtected))
	void SetTargetSlot(const int32 NewTargetSlot = -1);

	UFUNCTION(BlueprintCallable, Category = "UI|Inventory", meta = (BlueprintProtected))
	void SetItemData(const FItemSlot NewItemData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UI|Inventory")
	void UpdateGraphics();
	void UpdateGraphics_Implementation();
};
