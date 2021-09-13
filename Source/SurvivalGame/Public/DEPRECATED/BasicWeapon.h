// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DEPRECATED/BasicTool.h"
#include "InventoryTypes.h"
#include "BasicWeapon.generated.h"

class UBasicItemData;
class UItemData_SingleInt;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom))
class SURVIVALGAME_API ABasicWeapon : public ABasicTool
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Tool|Weapon", meta = (AllowPrivateAccess = "true"))
	FHitResult LastHitResult;

public:
	ABasicWeapon();

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Tool|Weapon", meta = (BlueprintProtected))
	int32 Damage;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Tool|Weapon", meta = (BlueprintProtected))
	float ToolRange;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Tool|Weapon", meta = (BlueprintProtected))
	UItemData_SingleInt* WeaponDataRef;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Tool|Weapon", meta = (BlueprintProtected))
	FItemSlot AmmoData;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Tool|Weapon", meta = (BlueprintProtected))
	int32 AmmoCount_MAX;

public:
	UFUNCTION(BlueprintCallable, Category = "Tool|Weapon")
	FHitResult GetLastHitResult() const;

	UFUNCTION(BlueprintCallable, Category = "Tool|Weapon")
	bool UpdateLastHitResult();

	virtual void SetupPlayerInputComponent() override;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Tool")
	bool CanReloadAction() const;
	virtual bool CanReloadAction_Implementation() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Tool")
	void OnReloadAction();
	virtual void OnReloadAction_Implementation();

	void OnReloadAction_Start();
	void OnReloadAction_End();

	UFUNCTION(BlueprintCallable, Category = "Tool|Weapon")
	virtual void ConsumeAmmo(int32 AmountToConsume = 1);

	UFUNCTION(BlueprintCallable, Category = "Tool|Weapon")
	virtual void SetCurrentAmmoCount(int32 NewAmmoCount);

	UFUNCTION(BlueprintCallable, Category = "Tool|Weapon")
	virtual int32 GetCurrentAmmoCount() const;
	
	virtual void OnPrimaryAction_Implementation() override;
	virtual void OnSecondaryAction_Implementation() override;
	virtual void OnTertiaryAction_Implementation() override;

	virtual void Initialize_Implementation(const int32 SlotIndex, UBasicItemData* NewItemInstanceData);
};
