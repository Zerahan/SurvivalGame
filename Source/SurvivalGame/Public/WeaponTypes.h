// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponTypes.generated.h"

USTRUCT(BlueprintType)
struct SURVIVALGAME_API FWeaponData {
	GENERATED_BODY()
	
	FWeaponData()
		: AmmoTypeHandle(FDataTableRowHandle())
		, AmmoConsumed(1)
		, AmmoCount_MAX(1)
		, ReloadSpeed(0.1f)
	{
		
	}
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	FDataTableRowHandle AmmoTypeHandle;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	int32 AmmoConsumed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	int32 AmmoCount_MAX;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	float ReloadSpeed;
};