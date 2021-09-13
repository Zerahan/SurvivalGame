// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryTypes.h"
#include "BasicInventoryReserverComponent.generated.h"

class UInventoryComponent;

UCLASS( Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURVIVALGAME_API UBasicInventoryReserverComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBasicInventoryReserverComponent();
	
private:
	UPROPERTY()
	APlayerController* OwningPlayerRef;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory|Reserve")
	UInventoryComponent* InventoryRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory|Reserve", meta = (BlueprintProtected))
	int32 NumSlotsToReserve;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory|Reserve")
	int32 ReservedIndex;
	
public:
	void SetNumSlotsToReserve(int32 NewNumSlotsToReserve);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Reserve")
	bool GetReservedInventorySlots(TArray<int32>& TargetSlots) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory|Reserve")
	bool GetReservedItemSlot(const int32 TargetSlot, FItemSlot& ItemData) const;
	
	UFUNCTION(BlueprintCallable, Category = "Inventory|Reserve")
	bool GetReservedItemSlot_WithStaticData(const int32 TargetSlot, int32& InventorySlotIndex, FItemSlot& ItemData, FItemStaticData& StaticData) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory|Reserve")
	APlayerController* GetOwningPlayer();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory|Reserve")
	bool CanAddToReservedSlot(const int32 TargetSlot, const FItemSlot ItemData);
	bool CanAddToReservedSlot_Implementation(const int32 TargetSlot, const FItemSlot ItemData);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory|Reserve")
	bool CanRemoveFromReservedSlot(const int32 TargetSlot, const FItemSlot ItemData);
	bool CanRemoveFromReservedSlot_Implementation(const int32 TargetSlot, const FItemSlot ItemData);
};
