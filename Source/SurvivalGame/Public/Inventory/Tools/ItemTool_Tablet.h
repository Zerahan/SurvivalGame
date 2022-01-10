// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/ItemTool_Basic.h"
#include "ItemTool_Tablet.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom))
class SURVIVALGAME_API AItemTool_Tablet : public AItemTool_Basic
{
	GENERATED_BODY()

public:
	AItemTool_Tablet();

public:
	virtual bool CanTertiaryAction_Implementation() const override;
};
