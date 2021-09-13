// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DEPRECATED/BasicInventoryReserverComponent.h"
#include "HotbarHandlerComponent.generated.h"

//DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnHotbarSelectionChangedDispatcher, UHotbarHandlerComponent, OnHotbarSelectionChangedDispatcher, int32, Index);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURVIVALGAME_API UHotbarHandlerComponent : public UBasicInventoryReserverComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHotbarHandlerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Control|Hotbar", meta = (BlueprintProtected))
	int32 SelectedIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Control|Hotbar", meta = (BlueprintProtected))
	USceneComponent* AttachmentRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Control|Hotbar", meta = (BlueprintProtected))
	FName AttachmentSocket;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Control|Hotbar", meta = (BlueprintProtected))
	ABasicTool* HandsToolRef;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Control|Hotbar", meta = (BlueprintProtected))
	FItemSlot SelectedItemData;
	
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Control|Hotbar", meta = (BlueprintProtected))
	ABasicTool* CurrentToolRef;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Control|Hotbar", meta = (BlueprintProtected))
	TArray<ABasicTool*> SpawnedToolRefs;

	UFUNCTION(BlueprintCallable, Category = "Control|Hotbar")
	int32 GetSelectedIndex() const;
	
	UFUNCTION(BlueprintCallable, Category = "Control|Hotbar")
	void SetAttachment(USceneComponent* Component, FName Socket = NAME_None);

	UFUNCTION(BlueprintCallable, Category = "Control|Hotbar")
	void SetSelectedItem(int32 NewIndex);

	UFUNCTION(BlueprintCallable, Category = "Control|Hotbar")
	void SelectNextItem();

	UFUNCTION(BlueprintCallable, Category = "Control|Hotbar")
	void SelectPreviousItem();

	UFUNCTION(BlueprintCallable, Category = "Control|Hotbar")
	void EquipItem();
	
	UFUNCTION(BlueprintCallable, Category = "Control|Hotbar")
	void UnequipItem(bool ShouldDestroyActor = false);

	UFUNCTION()
	void OnInventoryUpdated(const int32 TargetSlot);

//	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Control|Hotbar")
//	FOnHotbarSelectionChangedDispatcher OnHotbarSelectionChangedDispatcher;
};
