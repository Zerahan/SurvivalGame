// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DEPRECATED/BasicWeapon.h"
#include "Tool_Welder.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom))
class SURVIVALGAME_API ATool_Welder : public ABasicWeapon
{
	GENERATED_BODY()

public:
	ATool_Welder();

public:
	virtual bool CanPrimaryAction_Implementation() const override;
	virtual bool CanSecondaryAction_Implementation() const override;
	virtual bool CanReloadAction_Implementation() const override;
	virtual void OnReloadAction_Implementation() override;
	virtual void OnPrimaryAction_Implementation() override;
	virtual void OnSecondaryAction_Implementation() override;
	//virtual void Initialize_Implementation(UBasicItemData* NewItemInstanceData) override;
};
