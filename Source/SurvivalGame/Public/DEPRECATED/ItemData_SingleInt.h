// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DEPRECATED/BasicItemData.h"
#include "ItemData_SingleInt.generated.h"

/**
 * An ItemData that stores a single integer.
 * Primarily for items that store things like ammo, durability, item charge, etc.
 */
UCLASS(BlueprintType, Blueprintable)
class SURVIVALGAME_API UItemData_SingleInt : public UBasicItemData
{
	GENERATED_BODY()

	UItemData_SingleInt()
		: Stat1(0)
		, Stat1_MAX(10)
	{}

private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", meta = (AllowPrivateAccess = "true"))
	int32 Stat1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", meta = (AllowPrivateAccess = "true"))
	int32 Stat1_MAX;

public:
	UFUNCTION(BlueprintCallable, Category = "Data")
	int32 GetStat() const;

	UFUNCTION(BlueprintCallable, Category = "Data")
	void SetStat(const int32 NewStat);

	UFUNCTION(BlueprintCallable, Category = "Data")
	void ModStat(const int32 ModAmount);

	UFUNCTION(BlueprintCallable, Category = "Data")
	int32 GetStatMAX() const;

	UFUNCTION(BlueprintCallable, Category = "Data")
	void SetStatMAX(const int32 NewStatMAX);
};
