// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/DroppedItem_Basic.h"
#include "Inventory/InventorySlotData_Stack.h"
#include "DroppedItem_Generic.generated.h"

class UWidgetComponent;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom))
class INVENTORYSYSTEM_API ADroppedItem_Generic : public ADroppedItem_Basic
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	ADroppedItem_Generic();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	FItemSlotBuilder_Stack ItemDataBuilder;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Inventory")
	UStaticMeshComponent* RootStaticMeshComponent;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Inventory")
	UWidgetComponent* ItemDataDisplayComponent1;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Inventory")
	UWidgetComponent* ItemDataDisplayComponent2;

	virtual void UpdateGraphics_Implementation() override;
};
