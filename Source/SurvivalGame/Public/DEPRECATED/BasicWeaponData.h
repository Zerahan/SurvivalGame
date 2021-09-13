// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DEPRECATED/BasicToolData.h"
#include "BasicWeaponData.generated.h"

UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom))
class SURVIVALGAME_API UBasicWeaponData : public UBasicToolData {

	GENERATED_BODY()

public:
	UBasicWeaponData()
		: AmmoCount(0)
	{

	}

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tool|Weapon", meta = (BlueprintProtected))//, BlueprintGetter = &UBasicWeaponData::GetAmmoCount, BlueprintSetter = &UBasicWeaponData::SetAmmoCount));
	int32 AmmoCount;

public:
	UFUNCTION(BlueprintCallable, Category = "Tool|Weapon")
	void SetAmmoCount(const int32 NewAmmoCount);

	UFUNCTION(BlueprintCallable, Category = "Tool|Weapon")
	int32 GetAmmoCount() const;
};