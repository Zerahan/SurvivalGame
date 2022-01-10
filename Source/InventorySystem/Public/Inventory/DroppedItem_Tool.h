// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/DroppedItem_Basic.h"
#include "Inventory/InventorySlotData_Tool.h"
#include "DroppedItem_Tool.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom))
class INVENTORYSYSTEM_API ADroppedItem_Tool : public ADroppedItem_Basic
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	ADroppedItem_Tool();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	FItemSlotBuilder_Tool ItemDataBuilder;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Inventory")
	UStaticMeshComponent* RootStaticMeshComponent;

	virtual void UpdateGraphics_Implementation() override;
};
