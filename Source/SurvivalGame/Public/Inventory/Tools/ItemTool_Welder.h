// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/ItemTool_Basic.h"
#include "ItemTool_Welder.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom))
class SURVIVALGAME_API AItemTool_Welder : public AItemTool_Basic
{
	GENERATED_BODY()

public:
	AItemTool_Welder();

public:
	//virtual void OnPrimaryAction_Implementation() override;
	virtual bool CanSecondaryAction_Implementation() const override;
	virtual bool CanTertiaryAction_Implementation() const override;
};
