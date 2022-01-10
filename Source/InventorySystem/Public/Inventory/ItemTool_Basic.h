// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Item_Basic.h"
#include "Inventory/InventorySlotData_Tool.h"
#include "ItemTool_Basic.generated.h"

class ASurvivalCharacter;
class UItemData_Tool;
class UInputComponent;
class UInventoryManager;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom))
class INVENTORYSYSTEM_API AItemTool_Basic : public AItem_Basic
{
	GENERATED_BODY()

public:
	AItemTool_Basic();

private:
	UPROPERTY()
	bool IsEquipped;

	//UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Default|Display", meta = (AllowPrivateAccess = "true"))

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Tool", meta = (BlueprintProtected))
	FName ToolName;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default|Control", meta = (BlueprintProtected))
	FTransform AttachmentTransform;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default|Control", meta = (BlueprintProtected))
	EHoldingAnimation HoldAnimation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Control", meta = (BlueprintProtected))
	bool IsPrimaryActionAutomated;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Control", meta = (BlueprintProtected))
	float PrimaryActionRate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Control", meta = (BlueprintProtected))
	bool IsSecondaryActionAutomated;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Control", meta = (BlueprintProtected))
	float SecondaryActionRate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Control", meta = (BlueprintProtected))
	bool IsTertiaryActionAutomated;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Control", meta = (BlueprintProtected))
	float TertiaryActionRate;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Default|Control", meta = (BlueprintProtected))
	FTimerHandle PrimaryTimerHandle;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Default|Control", meta = (BlueprintProtected))
	FTimerHandle SecondaryTimerHandle;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Default|Control", meta = (BlueprintProtected))
	FTimerHandle TertiaryTimerHandle;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Default|Control|Hotbar", meta = (BlueprintProtected))
	UInventoryManager* InventoryRef;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Default|Control|Hotbar", meta = (BlueprintProtected))
	UInventorySlotData_Tool* ToolDataRef;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Default|Control|Hotbar", meta = (BlueprintProtected))
	FItemStaticData_Tool ToolStaticDataRef;

public:
	UFUNCTION(BlueprintCallable, Category = "Default|Tool")
	void SetIsEquipped(const bool NewIsEquipped);

	UFUNCTION(BlueprintCallable, Category = "Default|Tool")
	bool GetIsEquipped() const;

	UFUNCTION(BlueprintCallable, Category = "Default|Animation")
	EHoldingAnimation GetHoldType() const;

	UFUNCTION(BlueprintCallable, Category = "Default|Tool")
	FTransform GetAttachmentTransform() const;

	UFUNCTION(BlueprintCallable, Category = "Default|Tool")
	virtual void SetupPlayerInputComponent();

	UFUNCTION(BlueprintCallable, Category = "Tool")
	void SetOwnerInfo(APawn* NewOwningCharacterRef);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default|Tool")
	bool CanPrimaryAction() const;
	virtual bool CanPrimaryAction_Implementation() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default|Tool")
	void OnPrimaryAction();
	virtual void OnPrimaryAction_Implementation();

	void OnPrimaryAction_Start();
	void OnPrimaryAction_End();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default|Tool")
	bool CanSecondaryAction() const;
	virtual bool CanSecondaryAction_Implementation() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default|Tool")
	void OnSecondaryAction();
	virtual void OnSecondaryAction_Implementation();

	void OnSecondaryAction_Start();
	void OnSecondaryAction_End();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default|Tool")
	bool CanTertiaryAction() const;
	virtual bool CanTertiaryAction_Implementation() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default|Tool")
	void OnTertiaryAction();
	virtual void OnTertiaryAction_Implementation();

	void OnTertiaryAction_Start();
	void OnTertiaryAction_End();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default|Tool")
	void OnEquip(APlayerController* ControllerRef_);
	virtual void OnEquip_Implementation(APlayerController* ControllerRef_);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default|Tool")
	void OnUnequip(APlayerController* ControllerRef_, const bool ShouldDestroyActor = false);
	virtual void OnUnequip_Implementation(APlayerController* ControllerRef_, const bool ShouldDestroyActor = false);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default|Tool")
	void Initialize(UInventorySlotData_Tool* ItemInstanceData_);
	virtual void Initialize_Implementation(UInventorySlotData_Tool* ItemInstanceData_);
	
};
