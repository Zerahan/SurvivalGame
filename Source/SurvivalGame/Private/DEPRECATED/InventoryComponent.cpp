// Fill out your copyright notice in the Description page of Project Settings.

#include "DEPRECATED/InventoryComponent.h"
#include "InventoryDragDropPayload.h"
#include "DEPRECATED/BasicInventoryReserverComponent.h"
#include "Inventory/DroppedItem_Basic.h"
#include "Utility/UtilityFunctionLibrary.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	DisplayName = "Generic Inventory";
	InventorySize = 8;
	StackSizeOverrideMultiplier = 1.0f;
	HasReservedSlots = false;

	InventorySlots.SetNum(InventorySize);
	LastInventorySlotIndex = InventorySlots.Num() - 1;
}

void UInventoryComponent::DestroyComponent(bool bPromoteChildren)
{
	UActorComponent::DestroyComponent(bPromoteChildren);
}

void UInventoryComponent::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	//InventorySlots.Empty();
	InventorySlots.SetNum(InventorySize);
	LastInventorySlotIndex = InventorySlots.Num() - 1;
}

int32 UInventoryComponent::GetInventorySize(const bool IncludeReservedSlots) const
{
	if (HasReservedSlots){
		if (IncludeReservedSlots) {
			return InventorySlots.Num();
		} else {
			return LastInventorySlotIndex + 1;
		}
	} else {
		return InventorySlots.Num();
	}
}

void UInventoryComponent::SetSlotData(const int32 TargetSlot, const FItemSlot ItemData)
{
	if (IsSlotValid(TargetSlot)) {
		InventorySlots[TargetSlot] = ItemData;

		// Initializes instanced data if necissary
		if ( !IsValid(ItemData.InstancedDataRef) ) {
			FItemStaticData StaticData;
			if (LookupStaticData(ItemData, StaticData)) {
				if (StaticData.InstancedDataClass && StaticData.InstancedDataClass != UBasicItemData::StaticClass()) {
					InventorySlots[TargetSlot].InstancedDataRef = NewObject<UBasicItemData>(GetOwner(), StaticData.InstancedDataClass);
				}
			}
		}
		OnInventorySlotChangedDispatcher.Broadcast(TargetSlot);
	}
}

int32 UInventoryComponent::AddReservedInfo(const FReservedInfo ReservedInfo)
{
	if (FReservedInfo::IsValid(ReservedInfo) == false) {  return -1; }
	int32 index = ReservedSlots.Add(ReservedInfo);
	ReservedSlots[index].StartIndex = InventorySlots.Num();
	InventorySlots.AddDefaulted(ReservedSlots[index].NumSlots);
	return index;
}

bool UInventoryComponent::GetReservedInfo(const int32 ReservedSlotsIndex, FReservedInfo & ReservedInfo)
{
	if (ReservedSlots.IsValidIndex(ReservedSlotsIndex) == false) { return false; }
	ReservedInfo = ReservedSlots[ReservedSlotsIndex];
	if (FReservedInfo::IsValid(ReservedInfo)) {
		return true;
	}
	return false;
}

void UInventoryComponent::RemoveReservedInfo(const int32 ReservedSlotsIndex, const UBasicInventoryReserverComponent * ReserverRef, const bool DropRemovedItems)
{
	if (ReservedSlots.IsValidIndex(ReservedSlotsIndex)) {
		if (FReservedInfo::IsValid(ReservedSlots[ReservedSlotsIndex]) && IsValid(ReservedSlots[ReservedSlotsIndex].ReserverComponentRef) && ReservedSlots[ReservedSlotsIndex].ReserverComponentRef == ReserverRef) {
			if (DropRemovedItems) {
				FItemSlot ItemData;
				FItemStaticData StaticData;
				for (int32 i = 0; i < ReservedSlots[ReservedSlotsIndex].NumSlots; i++) {
					if (GetSlotData(ReservedSlots[ReservedSlotsIndex].StartIndex + i, ItemData)) {
						if (LookupStaticData(ItemData, StaticData)) {
							ADroppedItem_Basic* SpawnedObject = CreateDroppedItem(StaticData);
							if (SpawnedObject == nullptr) { continue; }
							//SpawnedObject->SetItemData(ItemData);
						}
					}
				}
			}
			InventorySlots.RemoveAt(ReservedSlots[ReservedSlotsIndex].StartIndex, ReservedSlots[ReservedSlotsIndex].NumSlots);
		}
	}
}

UBasicInventoryReserverComponent* UInventoryComponent::GetReserveComponent(const int32 TargetSlot) const
{
	if (IsSlotReserved(TargetSlot)) {
		for (int i = 0; i < ReservedSlots.Num(); i++) {
			if (TargetSlot >= ReservedSlots[i].StartIndex && TargetSlot < (ReservedSlots[i].StartIndex + ReservedSlots[i].NumSlots - 1)) {
				return ReservedSlots[i].ReserverComponentRef;
			}
		}
	}
	return nullptr;
}

void UInventoryComponent::SetLinkedInventoryRef(UInventoryComponent* NewLinkedInventoryRef)
{
	if (LinkedInventoryRef && IsValid(LinkedInventoryRef)) {
		LinkedInventoryRef->LinkedInventoryRef = nullptr;
		LinkedInventoryRef->OnLinkedInventoryDispatcher.Broadcast();
	}
	LinkedInventoryRef = NewLinkedInventoryRef;
	if (LinkedInventoryRef && IsValid(LinkedInventoryRef)) {
		LinkedInventoryRef->LinkedInventoryRef = this;
		OnLinkedInventoryDispatcher.Broadcast();
	}
}

FVector UInventoryComponent::GetWorldSpawnLocation()
{
	if (SpawnPointComponentRef && IsValid(SpawnPointComponentRef)) {
		return SpawnPointComponentRef->GetComponentLocation();
	}
	if (LinkedInventoryRef && IsValid(LinkedInventoryRef)) {
		return LinkedInventoryRef->GetWorldSpawnLocation();
	}
	return GetOwner()->GetActorLocation();
}

FRotator UInventoryComponent::GetWorldSpawnRotation()
{
	if (SpawnPointComponentRef && IsValid(SpawnPointComponentRef)) {
		return SpawnPointComponentRef->GetComponentRotation();
	}
	if (LinkedInventoryRef && IsValid(LinkedInventoryRef)) {
		return LinkedInventoryRef->GetWorldSpawnRotation();
	}
	return GetOwner()->GetActorRotation();
}

float UInventoryComponent::GetStackSizeWithOverride(const FItemStaticData StaticData) const
{
	if (StaticData.MaxStackSize > 1.0f) {
		return StaticData.MaxStackSize * StackSizeOverrideMultiplier;
	}
	return StaticData.MaxStackSize;
}

bool UInventoryComponent::GetSlotData(const int32 TargetSlot, FItemSlot& ReturnData) const
{
	if (IsSlotValid(TargetSlot)) {
		ReturnData = InventorySlots[TargetSlot];
		return IsItemSlotValid(ReturnData);
	}
	ReturnData = FItemSlot();
	return false;
}

bool UInventoryComponent::IsItemSlotValid(const FItemSlot & ItemData)
{
	return FItemSlot::IsValid(ItemData);
}

bool UInventoryComponent::LookupStaticData(const FItemSlot& ItemData, FItemStaticData& ReturnData)
{
	try {
		if (!FItemSlot::IsValid(ItemData)) { return false; }
		//FItemStaticData* StaticData = ItemData.StaticDataHandle.DataTable->FindRow<FItemStaticData>(ItemData.StaticDataHandle.RowName, "");
		//ReturnData = *StaticData;
		ReturnData = *ItemData.StaticDataHandle.GetRow<FItemStaticData>("");
		return FItemStaticData::IsValid(ReturnData);
	}
	catch (...) {}
	ReturnData = FItemStaticData();
	return false;
}



//-----------------------//
// Information Gathering //
//-----------------------//

bool UInventoryComponent::IsSlotValid(const int32 TargetSlot) const
{
	return InventorySlots.IsValidIndex(TargetSlot);
}

bool UInventoryComponent::IsSlotEmpty(const int32 TargetSlot) const
{
	FItemSlot ReturnData;
	if (IsSlotValid(TargetSlot)) {
		if (GetSlotData(TargetSlot, ReturnData)) {
			return !FItemSlot::IsValid(ReturnData);
		}
		return true;
	}
	return false;
}

bool UInventoryComponent::IsSlotOfType(const int32 TargetSlot, const FItemSlot& ItemData) const
{
	if (!FItemSlot::IsValid(ItemData)) { return false; }
	FItemSlot ReturnData;
	if (GetSlotData(TargetSlot, ReturnData)) {
		return ReturnData == ItemData;
	}
	return false;
}

bool UInventoryComponent::IsSlotReserved(const int32 TargetSlot) const
{
	if (IsSlotValid(TargetSlot) == false) { return true; }
	if (TargetSlot > LastInventorySlotIndex) { return true; }
	return false;
}

TArray<int32> UInventoryComponent::GetInventory(const bool IncludeReservedSlots) const
{
	TArray<int32> FilteredSlotsArray;
	int32 InvSize = GetInventorySize(IncludeReservedSlots);
	for (int i = 0; i < InvSize; i++) {
		FilteredSlotsArray.Add(i);
	}
	return FilteredSlotsArray;
}

float UInventoryComponent::GetTotalAmountOfType(const FItemSlot & ItemData) const
{
	if (!FItemSlot::IsValid(ItemData)) { return -1.f; }
	float TotalAmount = 0.0f;
	for (int i = 0; i < InventorySlots.Num(); i++) {
		if (IsSlotOfType(i, ItemData)) {
			//UUtilityFunctionLibrary::PrintDebug(FString::FromInt(i) + ") Slot is of type");
			TotalAmount += InventorySlots[i].Amount;
		}
	}
	return TotalAmount;
}

float UInventoryComponent::GetTotalAmountOfTypeAndSlots(const FItemSlot & ItemData, TArray<int32>& TargetSlots) const
{
	if (FItemSlot::IsValid(ItemData) == false) { return -1.f; }
	float TotalAmount = 0.0f;
	int32 InvSize = GetInventorySize();
	for (int i = 0; i < InvSize; i++) {
		if (IsSlotOfType(i, ItemData)) {
			TotalAmount += InventorySlots[i].Amount;
			TargetSlots.Add(i);
		}
	}
	return TotalAmount;
}

bool UInventoryComponent::FindAllEmptySlots(TArray<int32>& TargetSlots) const
{
	int32 InvSize = GetInventorySize();
	for (int i = 0; i < InvSize; i++) {
		if (IsSlotEmpty(i)) {
			TargetSlots.Add(i);
		}
	}
	return TargetSlots.Num() > 0;
}

bool UInventoryComponent::FindAllSlotsOfType(const FItemSlot & ItemData, TArray<int32>& TargetSlots) const
{
	if (FItemSlot::IsValid(ItemData) == false) { return false; }
	int32 InvSize = GetInventorySize();
	for (int i = 0; i < InvSize; i++) {
		if (IsSlotOfType(i, ItemData)) {
			TargetSlots.Add(i);
		}
	}
	return TargetSlots.Num() > 0;
}



//--------------//
// Manipulation //
//--------------//

bool UInventoryComponent::CanAddToInventory(const FItemSlot & ItemData) const
{
	if (FItemSlot::IsValid(ItemData) == false) { return false; }
	FItemStaticData StaticData;
	TArray<int32> TargetSlots;
	float TotalAmount = 0.0f;
	if (LookupStaticData(ItemData, StaticData)) {
		if (FindAllEmptySlots(TargetSlots)) {
			TotalAmount += GetStackSizeWithOverride(StaticData) * TargetSlots.Num();
			if (TotalAmount >= ItemData.Amount) {
				return true;
			}
		}
		TargetSlots.Empty();
		float Total = GetTotalAmountOfTypeAndSlots(ItemData, TargetSlots);

		TotalAmount += (GetStackSizeWithOverride(StaticData) * TargetSlots.Num()) - Total;
		if (TotalAmount >= ItemData.Amount) {
			return true;
		}
	}
	return false;
}

bool UInventoryComponent::CanRemoveFromInventory(const FItemSlot & ItemData)
{
	if ( !FItemSlot::IsValid(ItemData) ) { return false; }
	float TotalAmount = GetTotalAmountOfType(ItemData);
	return TotalAmount >= ItemData.Amount;
}



bool UInventoryComponent::AddToSlot(const int32 TargetSlot, const FItemSlot & ItemData, FItemSlot & Remainder)
{
	if (FItemSlot::IsValid(ItemData) == false) { return false; }
	Remainder = ItemData;
	if (IsSlotReserved(TargetSlot)){
		UBasicInventoryReserverComponent* ComponentRef = GetReserveComponent(TargetSlot);
		if (IsValid(ComponentRef) && ComponentRef->CanAddToReservedSlot(TargetSlot, ItemData) == false){
			return true;
		}
	}
	FItemSlot TargetSlotData;
	if (IsSlotValid(TargetSlot)) {
		FItemStaticData StaticData;
		if (!LookupStaticData(ItemData, StaticData)){ return true; }
		if (GetSlotData(TargetSlot, TargetSlotData)) {
			if (IsSlotOfType(TargetSlot, ItemData)) {
				if (GetStackSizeWithOverride(StaticData) < TargetSlotData.Amount + ItemData.Amount) {
					float Difference = GetStackSizeWithOverride(StaticData) - TargetSlotData.Amount;
					TargetSlotData.Amount = GetStackSizeWithOverride(StaticData);
					Remainder.Amount = ItemData.Amount - Difference;
					SetSlotData(TargetSlot, TargetSlotData);
					return true;
				}
				else {
					TargetSlotData.Amount += ItemData.Amount;
					SetSlotData(TargetSlot, TargetSlotData);
					return false;
				}
			}
		}
		else {
			TargetSlotData = ItemData;
			if (GetStackSizeWithOverride(StaticData) < ItemData.Amount) {
				TargetSlotData.Amount = GetStackSizeWithOverride(StaticData);
				SetSlotData(TargetSlot, TargetSlotData);
				Remainder.Amount = ItemData.Amount - GetStackSizeWithOverride(StaticData);
				return true;
			}
			else {
				SetSlotData(TargetSlot, TargetSlotData);
				return false;
			}
		}
	}
	return true;
}

bool UInventoryComponent::RemoveFromSlot(const int32 TargetSlot, const FItemSlot & ItemData, FItemSlot & Remainder)
{
	if (FItemSlot::IsValid(ItemData) == false) { return false; }
	Remainder = ItemData;
	if (IsSlotReserved(TargetSlot)) {
		UBasicInventoryReserverComponent* ComponentRef = GetReserveComponent(TargetSlot);
		if (IsValid(ComponentRef) && ComponentRef->CanRemoveFromReservedSlot(TargetSlot, ItemData) == false) {
			return true;
		}
	}
	FItemSlot TargetSlotData;
	if (GetSlotData(TargetSlot, TargetSlotData)) {
		if (FItemSlot::IsValid(TargetSlotData) && IsSlotOfType(TargetSlot, ItemData)) {
			if (TargetSlotData.Amount <= ItemData.Amount) {
				Remainder.Amount -= TargetSlotData.Amount;
				SetSlotData(TargetSlot, FItemSlot());
				return true;
			}
			else {
				TargetSlotData.Amount -= ItemData.Amount;
				SetSlotData(TargetSlot, TargetSlotData);
				return false;
			}
		}
	}
	return true;
}

bool UInventoryComponent::AddToInventory(const FItemSlot & ItemData, FItemSlot & Remainder)
{
	if (FItemSlot::IsValid(ItemData) == false) { return false; }
	Remainder = ItemData;
	TArray<int32> TargetSlots;
	FindAllSlotsOfType(ItemData, TargetSlots);
	FindAllEmptySlots(TargetSlots);
	FItemSlot WorkingData;
	WorkingData = ItemData;
	FItemSlot ReturnValue;
	for (int i = 0; i < TargetSlots.Num(); i++) {
		if (AddToSlot(TargetSlots[i], WorkingData, ReturnValue)) {
			WorkingData = ReturnValue;
		}
		else {
			WorkingData = FItemSlot();
			break;
		}
	}
	Remainder = WorkingData;
	//OnInventoryChangedDispatcher.Broadcast();
	if (FItemSlot::IsValid(Remainder)) {
		return true;
	}
	return false;
}

bool UInventoryComponent::RemoveFromInventory(const FItemSlot & ItemData, FItemSlot & Remainder)
{
	if (!FItemSlot::IsValid(ItemData)) { return false; }
	Remainder = ItemData;
	TArray<int32> TargetSlots;
	float TotalAmount = GetTotalAmountOfTypeAndSlots(ItemData, TargetSlots);
	if (TotalAmount >= ItemData.Amount) {
		FItemSlot WorkingData;
		WorkingData = ItemData;
		FItemSlot ReturnValue;
		for (int i = 0; i < TargetSlots.Num(); i++) {
			if (RemoveFromSlot(TargetSlots[i], WorkingData, ReturnValue)) {
				WorkingData = ReturnValue;
			}
			else {
				WorkingData = FItemSlot();
				break;
			}
		}
		Remainder = WorkingData;
		if (FItemSlot::IsValid(Remainder)) {
			//OnInventoryChangedDispatcher.Broadcast();
			return true;
		}
	}
	//OnInventoryChangedDispatcher.Broadcast();
	return false;
}

bool UInventoryComponent::TransferToSlot(const int32 ToSlot, UInventoryComponent* FromComponent, const int32 FromSlot, const float Amount /*= -1.f*/)
{
	FItemSlot FromSlotData;
	if (FromComponent->GetSlotData(FromSlot, FromSlotData)) {
		if (Amount > 0.0f && FromSlotData.Amount >= Amount) {
			FromSlotData.Amount = Amount;
		}
		FItemSlot Remainder;
		if (AddToSlot(ToSlot, FromSlotData, Remainder)) {
			FromSlotData.Amount -= Remainder.Amount;
			FromComponent->RemoveFromSlot(FromSlot, FromSlotData, Remainder);
			//OnInventoryChangedDispatcher.Broadcast();
			//FromComponent->OnInventoryChangedDispatcher.Broadcast();
			return true;
		}
		else {
			FromComponent->RemoveFromSlot(FromSlot, FromSlotData, Remainder);
			//OnInventoryChangedDispatcher.Broadcast();
			//FromComponent->OnInventoryChangedDispatcher.Broadcast();
			return true;
		}
	}
	//OnInventoryChangedDispatcher.Broadcast();
	return false;
}

bool UInventoryComponent::TransferToInventory(UInventoryComponent * FromComponent, const int32 FromSlot, const float Amount /*= -1.f*/)
{
	FItemSlot FromSlotData;
	if (FromComponent->GetSlotData(FromSlot, FromSlotData)) {
		if (Amount > 0.f && FromSlotData.Amount >= Amount) {
			FromSlotData.Amount = Amount;
		}
		FItemSlot Remainder;
		if (Amount == -2.f) {
			FromSlotData.Amount = FromComponent->GetTotalAmountOfType(FromSlotData);
			if (AddToInventory(FromSlotData, Remainder)) {
				FromSlotData.Amount -= Remainder.Amount;
			}
			FromComponent->RemoveFromInventory(FromSlotData, Remainder);
			//OnInventoryChangedDispatcher.Broadcast();
			//FromComponent->OnInventoryChangedDispatcher.Broadcast();
			return true;
		}
		else {
			if (AddToInventory(FromSlotData, Remainder)) {
				FromSlotData.Amount -= Remainder.Amount;
				FromComponent->RemoveFromSlot(FromSlot, FromSlotData, Remainder);
				//OnInventoryChangedDispatcher.Broadcast();
				//FromComponent->OnInventoryChangedDispatcher.Broadcast();
				return true;
			}
			else {
				FromComponent->RemoveFromSlot(FromSlot, FromSlotData, Remainder);
				//OnInventoryChangedDispatcher.Broadcast();
				//FromComponent->OnInventoryChangedDispatcher.Broadcast();
				return true;
			}
		}
	}
	return false;
}

bool UInventoryComponent::TransferSwapSlots(const int32 ToSlot, UInventoryComponent * FromComponent, const int32 FromSlot)
{
	if (FromComponent->IsSlotEmpty(FromSlot)) { return false; }
	FItemSlot FromSlotData;
	FItemSlot ToSlotData;
	if (FromComponent->GetSlotData(FromSlot, FromSlotData)) {
		FItemSlot Remainder;
		GetSlotData(ToSlot, ToSlotData);
		FromComponent->SetSlotData(FromSlot, ToSlotData);
		SetSlotData(ToSlot, FromSlotData);

		//OnInventoryChangedDispatcher.Broadcast();
		//FromComponent->OnInventoryChangedDispatcher.Broadcast();
		return true;
	}
	return false;
}

bool UInventoryComponent::DragDropHandler(const UInventoryDragDropPayload * Payload, const APawn * CallingPawn)
{
	/*
	// If AmountOverride is 0, there's no point in continuing.
	if (Payload->AmountOverride == 0.0f) { return false; }

	// If FromInventoryRef is invalid, there's no point in continuing.
	if (Payload->FromInventoryRef == nullptr) { return false; }

	FItemSlot FromSlotData;
	FItemSlot Remainder;

	// If FromInventoryRef is invalid, there's no point in continuing. Also gets the data from that slot for future use.
	if (Payload->FromInventoryRef->GetSlotData(Payload->FromSlot, FromSlotData) == false) { return false; }
	//float FromSlotAmount = FromSlotData.Amount;

	FItemStaticData StaticData;
	// Lookup data, if it fails, return unhandled.
	if (LookupStaticData(FromSlotData, StaticData) == false) { return false; }

	float LocalAmountOverride = Payload->AmountOverride;
	if ( StaticData.AmountIsInteger ) {
		LocalAmountOverride = FMath::CeilToFloat(LocalAmountOverride);
	}

	if (Payload->ToInventoryRef != nullptr && Payload->ToInventoryRef->IsValidLowLevel()) {
		// If ToSlot is -1, transfer FromSlot to anywhere in ToInventoryRef... Else...
		if (Payload->ToSlot == -1) {
			Payload->ToInventoryRef->TransferToInventory(Payload->FromInventoryRef, Payload->FromSlot, LocalAmountOverride);
		}
		// Else if ToSlot is a valid index...
		else if (Payload->ToInventoryRef->IsSlotValid(Payload->ToSlot)) {
			// If override is -2, transfer all of type from FromInventoryRef to anywhere in ToInventoryRef.
			if (LocalAmountOverride == -2.f) {
				FromSlotData.Amount = Payload->FromInventoryRef->GetTotalAmountOfType(FromSlotData);
				if (Payload->ToInventoryRef->AddToInventory(FromSlotData, Remainder)) {
					FromSlotData.Amount -= Remainder.Amount;
				}
				Payload->FromInventoryRef->RemoveFromInventory(FromSlotData, Remainder);
				return true;
			}
			// If ToSlot is of the same type or empty, transfer data from FromSlot
			else if (Payload->ToInventoryRef->IsSlotOfType(Payload->ToSlot, FromSlotData) || Payload->ToInventoryRef->IsSlotEmpty(Payload->ToSlot)) {
				Payload->ToInventoryRef->TransferToSlot(Payload->ToSlot, Payload->FromInventoryRef, Payload->FromSlot, LocalAmountOverride);
				return true;
			}
			// Else if ToSlot is of a different type and AmountOverride isn't set, swap data between FromSlot and ToSlot
			else if (LocalAmountOverride == -1.0f) {
				Payload->ToInventoryRef->TransferSwapSlots(Payload->ToSlot, Payload->FromInventoryRef, Payload->FromSlot);
				return true;
			}
		}
	}
	// If ToInventoryRef is invalid, spawn an actor version of the item.
	else {
		ADroppedItem_Basic* SpawnedObject = CreateDroppedItem(StaticData);

		if (SpawnedObject == nullptr) { return false; }

		// If AmountOverride is -1, move all of FromSlot into the spawned object.
		if (LocalAmountOverride == -1.0f) {
			if (SpawnedObject->SetItemData(FromSlotData)) {
				Payload->FromInventoryRef->SetSlotData(Payload->FromSlot, FItemSlot());
				//Payload->FromInventoryRef->OnInventoryChangedDispatcher.Broadcast();
			}
		}
		else {
			// If AmountOverride is -2, move all of FromSlot's item type from inventory into the spawned object
			if (LocalAmountOverride == -2.0f) {
				FromSlotData.Amount = Payload->FromInventoryRef->GetTotalAmountOfType(FromSlotData);
				if (Payload->FromInventoryRef->RemoveFromInventory(FromSlotData, Remainder)) {
					FromSlotData.Amount -= Remainder.Amount;
				}
			}
			// If AmountOverride is greater than 0, try to move that amount into the spawned object
			else if(LocalAmountOverride > 0.0f) {
				FromSlotData.Amount = LocalAmountOverride;
				if (Payload->FromInventoryRef->RemoveFromSlot(Payload->FromSlot,FromSlotData,Remainder) ) {
					FromSlotData.Amount -= Remainder.Amount;
					if (Payload->FromInventoryRef->RemoveFromInventory(FromSlotData, Remainder)) {
						FromSlotData.Amount -= Remainder.Amount;
					}
				}
			}
			SpawnedObject->SetItemData(FromSlotData);
		}
		return true;
	}
	//*/
	return false;
}

ADroppedItem_Basic* UInventoryComponent::CreateDroppedItem(const FItemStaticData& StaticData)
{
	if (FItemStaticData::IsValid(StaticData) == false) { return nullptr; }

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ADroppedItem_Basic* SpawnedObject;
	if (StaticData.SpawnedClass) {
		SpawnedObject = GetWorld()->SpawnActor<ADroppedItem_Basic>(StaticData.SpawnedClass, GetWorldSpawnLocation(), GetWorldSpawnRotation(), SpawnInfo);
	}
	else {
		SpawnedObject = GetWorld()->SpawnActor<ADroppedItem_Basic>(GetWorldSpawnLocation(), GetWorldSpawnRotation(), SpawnInfo);
	}

	if (SpawnedObject == nullptr) { return nullptr; }

	return SpawnedObject;
}
