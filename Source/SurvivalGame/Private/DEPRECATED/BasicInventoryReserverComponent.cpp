// Fill out your copyright notice in the Description page of Project Settings.


#include "DEPRECATED/BasicInventoryReserverComponent.h"
//#include "Components/ActorComponent.h"
//#include "InventoryTypes.h"
#include "DEPRECATED/InventoryComponent.h"

// Sets default values for this component's properties
UBasicInventoryReserverComponent::UBasicInventoryReserverComponent()
	:Super()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	NumSlotsToReserve = 1;
	ReservedIndex = -1;

	APawn* OwningPawn = Cast<APawn>(GetOwner());
	if (OwningPawn) {
		OwningPlayerRef = Cast<APlayerController>(OwningPawn->GetController());
	}
	// ...
}


// Called when the game starts
void UBasicInventoryReserverComponent::BeginPlay()
{
	Super::BeginPlay();
	FReservedInfo ReservedSlots = FReservedInfo();
	ReservedSlots.ReserverComponentRef = this;
	ReservedSlots.NumSlots = NumSlotsToReserve;
	InventoryRef = GetOwner()->FindComponentByClass<UInventoryComponent>();
	if (InventoryRef) {
		ReservedIndex = InventoryRef->AddReservedInfo(ReservedSlots);
		if (ReservedIndex == -1) {
			// An error has happened. How to handle it?
		}
	}
	// ...
	
}

inline void UBasicInventoryReserverComponent::SetNumSlotsToReserve(int32 NewNumSlotsToReserve) { NumSlotsToReserve = NewNumSlotsToReserve; }

bool UBasicInventoryReserverComponent::GetReservedInventorySlots(TArray<int32>& TargetSlots) const
{
	FReservedInfo ReservedInfo;
	if (InventoryRef->GetReservedInfo(ReservedIndex, ReservedInfo)) {
		for (int i = 0; i < ReservedInfo.NumSlots; i++) {
			TargetSlots.Add(ReservedInfo.StartIndex + i);
		}
		return true;
	}
	return false;
}

bool UBasicInventoryReserverComponent::GetReservedItemSlot(const int32 TargetSlot, FItemSlot& ItemData) const
{
	FReservedInfo ReservedInfo;
	if (InventoryRef->GetReservedInfo(ReservedIndex, ReservedInfo)) {
		if (InventoryRef->GetSlotData(ReservedInfo.StartIndex + TargetSlot, ItemData)) {
			return true;
		}
	}
	return false;
}

bool UBasicInventoryReserverComponent::GetReservedItemSlot_WithStaticData(const int32 TargetSlot, int32& InventorySlotIndex, FItemSlot& ItemData, FItemStaticData& StaticData) const
{
	FReservedInfo ReservedInfo;
	if (InventoryRef->GetReservedInfo(ReservedIndex, ReservedInfo)) {
		InventorySlotIndex = ReservedInfo.StartIndex + TargetSlot;
		if (InventoryRef->GetSlotData(InventorySlotIndex, ItemData)) {
			if (InventoryRef->LookupStaticData(ItemData, StaticData)) {
				return true;
			}
		}
	}
	return false;
}

APlayerController* UBasicInventoryReserverComponent::GetOwningPlayer()
{
	if (IsValid(OwningPlayerRef)) {
		return OwningPlayerRef;
	}
	else {
		APawn* OwningPawn = Cast<APawn>(GetOwner());
		if (OwningPawn) {
			OwningPlayerRef = Cast<APlayerController>(OwningPawn->GetController());
			if (IsValid(OwningPlayerRef)) {
				return OwningPlayerRef;
			}
		}
	}
	return nullptr;
}

bool UBasicInventoryReserverComponent::CanAddToReservedSlot_Implementation(const int32 TargetSlot, const FItemSlot ItemData)
{
	return true;
}

bool UBasicInventoryReserverComponent::CanRemoveFromReservedSlot_Implementation(const int32 TargetSlot, const FItemSlot ItemData)
{
	return true;
}

