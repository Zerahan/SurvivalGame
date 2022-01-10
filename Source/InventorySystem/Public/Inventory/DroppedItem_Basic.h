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

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory")
	UInventorySlotData_Basic* ItemData;

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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory")
	void UpdateGraphics();
	virtual void UpdateGraphics_Implementation();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Inventory")
	FOnUpdateGraphics OnUpdateGraphicsDispatcher;
};

/*
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inventory/InventoryTypes.h"
#include "Interfaces/InteractionInterface.h"
#include "DroppedItem_Basic.generated.h"

class UWidgetComponent;
class UDEPRECATED_BasicItemSlotWidget;

UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API ADroppedItem_Basic : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADroppedItem_Basic();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Inventory")
	UStaticMeshComponent* RootStaticMeshComponent;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Inventory")
	UWidgetComponent* ItemDataDisplayComponent1;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Inventory")
	UWidgetComponent* ItemDataDisplayComponent2;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//virtual void PostInitializeComponents() override;
	//virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	//virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory")
	AController* InteractingControllerRef;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory", meta = (ExposeOnSpawn = "true"))
	FItemSlot ItemData = FItemSlot();

	UFUNCTION(Blueprintcallable, Category = "Inventory")
	virtual bool SetItemData(const FItemSlot& NewItemData);

	bool CanInteract_Implementation(const FInteractionInfo Info) const override;
	void BeginInteraction_Implementation(const FInteractionInfo Info) override;
	void EndInteraction_Implementation(const FInteractionInfo Info) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory")
	void UpdateGraphics();
	void UpdateGraphics_Implementation();
};
//*/