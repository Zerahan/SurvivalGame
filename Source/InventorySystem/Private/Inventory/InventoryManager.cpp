// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryManager.h"
#include "Components/ActorComponent.h"
#include "Inventory/InventorySlotData_Stack.h"
#include "Inventory/InventorySlotData_Tool.h"
#include "Inventory/DroppedItem_Basic.h"
#include "Inventory/DroppedItem_Generic.h"
#include "Inventory/InventoryManager_DragPayload.h"
//#include "InventoryDragDropPayload.h"
#include "Engine/DataTable.h"
#include "Inventory/InventoryTypes.h"

// Sets default values for this component's properties
UInventoryManager::UInventoryManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	InventorySize	= 4;
	BanOnlyAllowedItemTypes = true;
	// ...
}

// Called when the game starts
void UInventoryManager::BeginPlay()
{
	Super::BeginPlay();
	InventorySlots.SetNum(InventorySize);
	// ...
}

void UInventoryManager::SetInventorySize(const int32 NewSize)
{
	InventorySize = NewSize;
}

void UInventoryManager::SetDisplayName(const FString NewName)
{
	DisplayName = NewName;
}

FString UInventoryManager::GetDisplayName() const { return DisplayName; }

void UInventoryManager::SetLinkedInventory(UInventoryManager* NewLinkedInventoryRef)
{
	if (LinkedInventoryRef != NewLinkedInventoryRef) {
		LinkedInventoryRef = NewLinkedInventoryRef;
		OnLinkedInventoryChangedDispatcher.Broadcast();
	}
}

UInventoryManager* UInventoryManager::GetLinkedInventory() const
{
	return LinkedInventoryRef;
}

FVector UInventoryManager::GetWorldSpawnLocation() const
{
	if (IsValid(SpawnPointComponentRef)) {
		return SpawnPointComponentRef->GetComponentLocation();
	}
	return GetOwner()->GetActorLocation() + GetOwner()->GetActorRotation().RotateVector(SpawnPointRef);
}

FRotator UInventoryManager::GetWorldSpawnRotation() const
{
	if (IsValid(SpawnPointComponentRef)) {
		return SpawnPointComponentRef->GetComponentRotation();
	}
	return GetOwner()->GetActorRotation();
}

int32 UInventoryManager::GetInventorySize() const
{
	return InventorySize;
}

UInventorySlotData_Basic* UInventoryManager::GetSlot(const int32 TargetSlot) const
{
	if (InventorySlots.IsValidIndex(TargetSlot)) {
		return InventorySlots[TargetSlot];
	}
	return nullptr;
}

int32 UInventoryManager::GetIndex(UInventorySlotData_Basic* SlotRef) const
{
	int32 index = -1;
	if (InventorySlots.Find(SlotRef, index)) {
		return index;
	}
	return -1;
}

void UInventoryManager::SetSlot(const int32 TargetSlot, UInventorySlotData_Basic* NewInventorySlotRef, const bool DoOverride)
{
	if (InventorySlots.IsValidIndex(TargetSlot)) {
		if (!IsValid(GetSlot(TargetSlot)) || DoOverride) {
			if (IsValid(InventorySlots[TargetSlot])) {
				InventorySlots[TargetSlot]->ConditionalBeginDestroy();
				InventorySlots[TargetSlot] = nullptr;
			}
			InventorySlots[TargetSlot] = NewInventorySlotRef;
			if (IsValid(InventorySlots[TargetSlot]) && InventorySlots[TargetSlot]->GetOuter() != this) {
				// Changes the outer of the incoming slot to be this component
				InventorySlots[TargetSlot]->Rename(nullptr, this);
			}
			OnInventoryManagerSlotChanged(TargetSlot);
		}
	}
}

bool UInventoryManager::IsSlotEmpty(const int32 TargetSlot) const
{
	return !IsValid(GetSlot(TargetSlot));
}

bool UInventoryManager::IsSlotOfType(const int32 TargetSlot, const FDataTableRowHandle StaticDataHandle) const
{
	if (IsValid(GetSlot(TargetSlot))) {
		return InventorySlots[TargetSlot]->GetStaticDataHandle() == StaticDataHandle;
	}
	return StaticDataHandle.IsNull();
}

bool UInventoryManager::ContainsAtLeast(const int32 AmountToFind, const FDataTableRowHandle StaticDataHandle, TArray<int32>& TargetSlots) const
{
	int32 CountedAmount = 0;
	int i = InventorySize;
	while (CountedAmount < AmountToFind && --i >= 0) {
		if (IsValid(InventorySlots[i])) {
			if (InventorySlots[i]->GetStaticDataHandle() == StaticDataHandle) {
				TargetSlots.Add(i);
				UInventorySlotData_Stack* StackSlot = Cast<UInventorySlotData_Stack>(InventorySlots[i]);
				if (StackSlot) {
					CountedAmount += StackSlot->GetAmount();
				}
				else {
					CountedAmount++;
				}
			}
		}
	}
	return CountedAmount >= AmountToFind;
}

bool UInventoryManager::NextSlotOfType(int32& TargetSlot, const FDataTableRowHandle& StaticDataHandle)
{
	bool ResultFound = false;
	int32 Iterator = TargetSlot;
	if (InventorySlots.IsValidIndex(TargetSlot)) {
		do {
			if (IsSlotOfType(Iterator, StaticDataHandle)) {
				ResultFound = true;
			}
		} while (ResultFound == false && InventorySlots.IsValidIndex(Iterator++));
		TargetSlot = Iterator;
		return ResultFound;
	}
	else {
		return false;
	}
}

bool UInventoryManager::PreviousSlotOfType(int32& TargetSlot, const FDataTableRowHandle& StaticDataHandle)
{
	bool WasResultFound = false;
	do {
		if (IsSlotOfType(TargetSlot, StaticDataHandle)) {
			WasResultFound = true;
		}
		else {
			TargetSlot--;
		}
	} while (!WasResultFound && TargetSlot >= 0);
	return WasResultFound;
}

void UInventoryManager::FindSlotsOfType(const FDataTableRowHandle& StaticDataHandle, TArray<int32>& TargetSlots) const
{
	for (int i = 0; i < InventorySlots.Num(); i++) {
		if (IsValid(InventorySlots[i])) {
			InventorySlots[i]->GetStaticDataHandle() == StaticDataHandle;
			TargetSlots.Add(i);
		}
	}
}

void UInventoryManager::FindEmptySlots(TArray<int32>& TargetSlots) const
{
	for (int i = 0; i < InventorySlots.Num(); i++) {
		if (!IsValid(InventorySlots[i])) {
			TargetSlots.Add(i);
		}
	}
}

bool UInventoryManager::AddToSlot(const int32 TargetSlot, const FItemData_Simple& SimpleData, FItemData_Simple& Remainder)
{
	if (InventorySlots.IsValidIndex(TargetSlot) && FItemData_Simple::IsValid(SimpleData)) {
		AddToSlot_Internal(TargetSlot, SimpleData, Remainder);
	}
	else {
		Remainder = SimpleData;
	}
	return Remainder.Amount > 0;
}

bool UInventoryManager::AddToSlot_Internal(const int32 TargetSlot, const FItemData_Simple& SimpleData, FItemData_Simple& Remainder)
{
	if (IsValid(InventorySlots[TargetSlot])) {
		InventorySlots[TargetSlot]->AddData(SimpleData, Remainder);
	}
	else {
		UInventorySlotData_Basic* NewSlot = NewObject<UInventorySlotData_Basic>(this, SimpleData.StaticDataHandle.GetRow<FItemStaticData_Basic>("Creating new Slot Object for Inventory")->InstancedDataClass);
		InventorySlots[TargetSlot] = NewSlot;
		NewSlot->AddData(SimpleData, Remainder);
	}
	if (SimpleData.Amount != Remainder.Amount) {
		OnInventoryManagerSlotChanged(TargetSlot);
	}
	return Remainder.Amount > 0;
}

bool UInventoryManager::RemoveFromSlot(const int32 TargetSlot, const FItemData_Simple& SimpleData, FItemData_Simple& Remainder)
{
	if (InventorySlots.IsValidIndex(TargetSlot) && FItemData_Simple::IsValid(SimpleData)) {
		RemoveFromSlot_Internal(TargetSlot, SimpleData, Remainder);
	}
	else {
		Remainder = SimpleData;
	}
	return Remainder.Amount > 0;
}

bool UInventoryManager::RemoveFromSlot_Internal(const int32 TargetSlot, const FItemData_Simple& SimpleData, FItemData_Simple& Remainder)
{
	if (IsValid(InventorySlots[TargetSlot])) {
		InventorySlots[TargetSlot]->RemoveData(SimpleData, Remainder);
		if (InventorySlots[TargetSlot]->ShouldDestroyObject()) {
			InventorySlots[TargetSlot]->ConditionalBeginDestroy();
			InventorySlots[TargetSlot] = nullptr;
		}
	}
	else {
		Remainder = SimpleData;
	}
	if (SimpleData.Amount != Remainder.Amount) {
		OnInventoryManagerSlotChanged(TargetSlot);
	}
	return Remainder.Amount > 0;
}

bool UInventoryManager::MergeToSlot(const int32 TargetSlot, UInventorySlotData_Basic* OtherSlotRef)
{
	int32 Def = -1;
	return MergeToSlot(TargetSlot, OtherSlotRef, Def);
}

bool UInventoryManager::MergeToSlot(const int32 TargetSlot, UInventorySlotData_Basic* InventorySlotRef, int32& Remainder)
{
	if (!IsValid(InventorySlotRef)) { return false; }
	UInventorySlotData_Basic* FoundSlot = GetSlot(TargetSlot);
	if (IsValid(FoundSlot) && FoundSlot->CanMergeData(InventorySlotRef)) {
		FoundSlot->MergeData(InventorySlotRef, Remainder);
		if (FoundSlot->ShouldDestroyObject()) {
			FoundSlot->ConditionalBeginDestroy();
		}
		OnInventoryManagerSlotChanged(TargetSlot);
		return true;
	}
	return false;
}

bool UInventoryManager::AddToInventory(const FItemData_Simple& SimpleData, FItemData_Simple& Remainder)
{
	if (FItemData_Simple::IsValid(SimpleData)) {
		int32 TargetSlot = 0; // Defaults to adding from the start of the InventorySlots array
		FItemData_Simple WorkingData = SimpleData;
		bool IsFinished = false;
		while (NextSlotOfType(TargetSlot, SimpleData.StaticDataHandle) && WorkingData.Amount > 0) {
			AddToSlot_Internal(TargetSlot, WorkingData, Remainder);
			WorkingData = Remainder;
			TargetSlot++;
		}
		if (Remainder.Amount > 0) {
			TargetSlot = 0;
			FDataTableRowHandle NullHandle;
			while (NextSlotOfType(TargetSlot, NullHandle) && WorkingData.Amount > 0) {
				AddToSlot_Internal(TargetSlot, WorkingData, Remainder);
				WorkingData = Remainder;
				TargetSlot++;
			}
		}
	}
	else {
		Remainder = SimpleData;
	}
	return Remainder.Amount > 0;
}

bool UInventoryManager::RemoveFromInventory(const FItemData_Simple& SimpleData, FItemData_Simple& Remainder)
{
	if (FItemData_Simple::IsValid(SimpleData)) {
		int32 TargetSlot = InventorySize-1; // Defaults to removing from the end of the InventorySlots array
		FItemData_Simple WorkingData = SimpleData;
		while (PreviousSlotOfType(TargetSlot, SimpleData.StaticDataHandle) && WorkingData.Amount > 0) {
			RemoveFromSlot_Internal(TargetSlot, WorkingData, Remainder);
			WorkingData = Remainder;
			TargetSlot--;
		}
	}
	else {
		Remainder = SimpleData;
	}
	return Remainder.Amount > 0;
}

bool UInventoryManager::MergeToInventory(UInventorySlotData_Basic* OtherSlotRef, int32& Remainder)
{
	if (IsValid(OtherSlotRef)) {
		int32 TargetSlot = 0;
		int32 WorkingValue = Remainder;
		FDataTableRowHandle StaticDataHandle = OtherSlotRef->GetStaticDataHandle();
		while (NextSlotOfType(TargetSlot, StaticDataHandle) && IsValid(OtherSlotRef) && (Remainder < 0 || WorkingValue > 0)) {
			MergeToSlot(TargetSlot, OtherSlotRef, WorkingValue);
			if (OtherSlotRef->ShouldDestroyObject()) {
				OtherSlotRef->ConditionalBeginDestroy();
			}
			TargetSlot++;
		}
		if (Remainder > 0) {
			Remainder = WorkingValue;
		}
		StaticDataHandle = FDataTableRowHandle();
		TargetSlot = 0;
		if (IsValid(OtherSlotRef)) {
			if (NextSlotOfType(TargetSlot, StaticDataHandle)) {
				SetSlot(TargetSlot, OtherSlotRef);
				return true;
			}
		}
		else {
			return true;
		}
	}
	return false;
}

/**
* Transfer types
*	Transfer entire slot to target slot
*	Transfer entire slot to any available slots
*	Transfer part of slot to target slot
*	Transfer part of slot to any available slots
*	Transfer all of slot's type in inventory to target slot
*	Transfer all of slot's type in inventory to any available slots
*/

// Transfers all of part of slot to target slot
bool UInventoryManager::TransferToSlot(const int32 ToSlot, UInventoryManager* FromInventoryRef, const int32 FromSlot, const float AmountOverride)
{
	if (InventorySlots.IsValidIndex(ToSlot)) {
		UInventorySlotData_Basic* SlotRef = FromInventoryRef->GetSlot(FromSlot);
		if (IsValid(SlotRef)) {
			int32 Remainder = AmountOverride;
			return MergeToSlot(ToSlot, SlotRef, Remainder);
		}
	}
	return false;
}

bool UInventoryManager::TransferToInventory(UInventoryManager* FromInventoryRef, const int32 FromSlot, const float AmountOverride)
{
	if (this == FromInventoryRef) { return false; }
	if (IsValid(FromInventoryRef->GetSlot(FromSlot))) {
		UInventorySlotData_Basic* SlotRef = FromInventoryRef->GetSlot(FromSlot);
		int32 Remainder = AmountOverride;
		if (AmountOverride == -1) {
			// Move FromSlot only
			MergeToInventory(SlotRef, Remainder);
			if (!IsValid(FromInventoryRef->GetSlot(FromSlot)) || SlotRef->GetOuter() != FromInventoryRef) {
				FromInventoryRef->SetSlot(FromSlot, nullptr, true);
			}
			return true;
		}
		else if(AmountOverride == -2){
			// Move all of type, starting with FromSlot
			FDataTableRowHandle StaticDataHandle = SlotRef->GetStaticDataHandle();
			MergeToInventory(FromInventoryRef->GetSlot(FromSlot), Remainder);
			if (!IsValid(FromInventoryRef->GetSlot(FromSlot)) || SlotRef->GetOuter() != FromInventoryRef) {
				FromInventoryRef->SetSlot(FromSlot, nullptr, true);
			}
			int32 TargetSlot = FromInventoryRef->GetInventorySize() - 1;
			bool CanContinue = true;
			Remainder = -1;
			while (FromInventoryRef->PreviousSlotOfType(TargetSlot, StaticDataHandle) && CanContinue) {
				CanContinue = MergeToInventory(FromInventoryRef->GetSlot(TargetSlot), Remainder);
				if (!IsValid(FromInventoryRef->GetSlot(FromSlot)) || SlotRef->GetOuter() != FromInventoryRef) {
					FromInventoryRef->SetSlot(FromSlot, nullptr, true);
				}
				TargetSlot--;
			}
			return true;
		}
		else {
			// Move up to AmountOverride, starting with FromSlot
			FDataTableRowHandle StaticDataHandle = SlotRef->GetStaticDataHandle();
			MergeToInventory(FromInventoryRef->GetSlot(FromSlot), Remainder);
			if (!IsValid(FromInventoryRef->GetSlot(FromSlot)) || SlotRef->GetOuter() != FromInventoryRef) {
				FromInventoryRef->SetSlot(FromSlot, nullptr, true);
			}
			if (Remainder <= 0) { return true; }
			int32 TargetSlot = FromInventoryRef->GetInventorySize() - 1;
			bool CanContinue = true;
			while (FromInventoryRef->PreviousSlotOfType(TargetSlot, StaticDataHandle) && CanContinue && Remainder > 0) {
				CanContinue = MergeToInventory(FromInventoryRef->GetSlot(TargetSlot), Remainder);
				if (!IsValid(FromInventoryRef->GetSlot(FromSlot)) || SlotRef->GetOuter() != FromInventoryRef) {
					FromInventoryRef->SetSlot(FromSlot, nullptr, true);
				}
				TargetSlot--;
			}
			return Remainder != AmountOverride;
		}
		//FDataTableRowHandle StaticDataHandle = FromInventoryRef->GetSlot(FromSlot)->GetStaticDataHandle();
	}
	return false;
}

bool UInventoryManager::TransferSwapSlots(const int32 ToSlot, UInventoryManager* FromInventoryRef, const int32 FromSlot)
{
	if (InventorySlots.IsValidIndex(ToSlot)){
		UInventorySlotData_Basic* SlotRef = FromInventoryRef->GetSlot(FromSlot);
		if (IsValid(SlotRef)) {
			FromInventoryRef->SetSlot(FromSlot, GetSlot(ToSlot), true);
			SetSlot(ToSlot, SlotRef, true);
			SlotRef = nullptr;
			return true;
		}
	}
	return false;
}

bool UInventoryManager::HandleDragDropPayload(const UInventoryManager_DragPayload* Payload, const APawn* CallingPawn)
{
	/*
	Payload->FromInventoryManagerRef;
	Payload->ToInventoryManagerRef;
	Payload->FromInventorySlot;
	Payload->ToInventorySlot;
	Payload->AmountOverride;
	Payload->ItemData;
	* If AmountOverride == -1, move all of FromInventorySlot
	* If AmountOverride == -2, move all of FromInventorySlot's type
	* If AmountOverride > 0, move up to override of FromInventorySlot's type to inventory
	* 
	* If ToInventorySlot == -1 move to anywhere (Aka, the user shift-clicked the item instead of dragging it)
	* If ToInventorySlot != -1 fill ToInventorySlot first
	*/
	if (!IsValid(Payload->FromInventoryManagerRef)) { return false; }
	if (GetOuter() != CallingPawn && Payload->FromInventoryManagerRef->GetOuter() != CallingPawn) { return false; }
	if (!IsValid(Payload->FromInventoryManagerRef->GetSlot(Payload->FromInventorySlot))) { return false; }
	if (IsValid(Payload->ToInventoryManagerRef)) {
		if (Payload->ToInventorySlot == -1) {
			return TransferToInventory(Payload->FromInventoryManagerRef, Payload->FromInventorySlot, Payload->AmountOverride);
		}
		else {
			if (!InventorySlots.IsValidIndex(Payload->ToInventorySlot)) { return false; }
			if (IsValid(InventorySlots[Payload->ToInventorySlot]) && InventorySlots[Payload->ToInventorySlot]->CanMergeData(Payload->FromInventoryManagerRef->GetSlot(Payload->FromInventorySlot))) {
				return TransferToSlot(Payload->ToInventorySlot, Payload->FromInventoryManagerRef, Payload->FromInventorySlot, Payload->AmountOverride);
			}
			else {
				return TransferSwapSlots(Payload->ToInventorySlot, Payload->FromInventoryManagerRef, Payload->FromInventorySlot);
			}
		}
	}
	else {
		//FDataTableRowHandle StaticDataHandle = Payload->FromInventoryRef->GetSlot(Payload->FromSlot)->GetStaticDataHandle();
		FItemStaticData_Basic* StaticData = Payload->FromInventoryManagerRef->GetSlot(Payload->FromInventorySlot)->GetStaticDataHandle().GetRow<FItemStaticData_Basic>("Creating dropped item from StaticData");
		ADroppedItem_Basic* DroppedItem = CreateDroppedItem(StaticData->SpawnedClass);
		if (DroppedItem) {
			DroppedItem->SetItemSlot(Payload->FromInventoryManagerRef->GetSlot(Payload->FromInventorySlot));
			if (Payload->FromInventoryManagerRef->GetSlot(Payload->FromInventorySlot)->GetOuter() != Payload->FromInventoryManagerRef) {
				Payload->FromInventoryManagerRef->SetSlot(Payload->FromInventorySlot, nullptr, true);
			}
			return true;
		}
	}
	return false;
}

ADroppedItem_Basic* UInventoryManager::CreateDroppedItem(const TSubclassOf<ADroppedItem_Basic> SpawnedClass)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ADroppedItem_Basic* SpawnedObject = nullptr;
	if (SpawnedClass) {
		SpawnedObject = GetWorld()->SpawnActor<ADroppedItem_Basic>(SpawnedClass, GetWorldSpawnLocation(), GetWorldSpawnRotation(), SpawnInfo);
	}
	else {
		SpawnedObject = GetWorld()->SpawnActor<ADroppedItem_Basic>(ADroppedItem_Generic::StaticClass(), GetWorldSpawnLocation(), GetWorldSpawnRotation(), SpawnInfo);
	}

	return SpawnedObject;
}

APlayerController* UInventoryManager::GetOwningPlayer()
{
	if(IsValid(OwningPlayerRef)) return OwningPlayerRef;

	APawn* OwningPawn = Cast<APawn>(GetOwner());
	if (OwningPawn) {
		OwningPlayerRef = Cast<APlayerController>(OwningPawn->GetController());
	}
	return OwningPlayerRef;
}

void UInventoryManager::OnInventoryManagerSlotChanged_Implementation(const int32 TargetSlot)
{
	OnInventoryManagerSlotChangedDispatcher.Broadcast(TargetSlot);
}
