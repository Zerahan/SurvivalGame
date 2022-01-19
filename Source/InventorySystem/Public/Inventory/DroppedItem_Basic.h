// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "DroppedItem_Basic.generated.h"

class UInventorySlotData_Basic;

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FOnUpdateGraphics, ADroppedItem_Basic, OnUpdateGraphicsDispatcher);

UCLASS(BlueprintType, Blueprintable, Abstract, ClassGroup = (Custom))
class INVENTORYSYSTEM_API ADroppedItem_Basic : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADroppedItem_Basic();

	// The item this actor is containing.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory")
	UInventorySlotData_Basic* ItemData;

	// The controller currently interacting with this actor.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory")
	AController* InteractingControllerRef;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UInventorySlotData_Basic* GetItemSlot() const;
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetItemSlot(UInventorySlotData_Basic* NewItemData);

	bool CanInteract_Implementation(const FInteractionInfo Info) const override;
	void BeginInteraction_Implementation(const FInteractionInfo Info) override;
	void EndInteraction_Implementation(const FInteractionInfo Info) override;

	/*
	* Purely for graphical changes/updates.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory")
	void UpdateGraphics();
	virtual void UpdateGraphics_Implementation();

	// The event dispatcher that is called when UpdateGraphics is called. Primarily for UI updates.
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Inventory")
	FOnUpdateGraphics OnUpdateGraphicsDispatcher;
};