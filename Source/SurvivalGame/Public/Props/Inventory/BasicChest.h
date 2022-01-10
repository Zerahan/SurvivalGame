// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SurvivalGame.h"
#include "GameFramework/Actor.h"
#include "Inventory/InventoryTypes.h"
#include "Interfaces/InteractionInterface.h"
#include "BasicChest.generated.h"

class UInventoryManager;

UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom))
class SURVIVALGAME_API ABasicChest : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	UInventoryManager* InventoryManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	AController* InteractingControllerRef;
	
public:
	ABasicChest();

public:
	bool CanInteract_Implementation(const FInteractionInfo Info) const override;
	void BeginInteraction_Implementation(const FInteractionInfo Info) override;
	void CancelInteraction_Implementation(const FInteractionInfo Info) override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ForceCancelInteraction();
};
