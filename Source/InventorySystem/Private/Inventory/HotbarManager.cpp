// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/HotbarManager.h"
#include "Inventory/InventorySlotData_Tool.h"
#include "Inventory/ItemTool_Basic.h"

UHotbarManager::UHotbarManager()
	:Super()
{
	SelectedIndex = 0;
}

void UHotbarManager::BeginPlay()
{
	Super::BeginPlay();
	if (HandStaticData.IsNull()) {
		return;
	}
	HandSlotData = UInventorySlotData_Tool::CreateNewSlotFromHandle(HandStaticData, this);
	SpawnedEquipment.SetNum(GetInventorySize());

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AItemTool_Basic* SpawnedObject = nullptr;
	FItemStaticData_Tool HandStatic = UInventorySlotData_Tool::LookupStaticData(HandSlotData);
	if (FItemStaticData_Tool::IsValid(HandStatic)) {
		SpawnedObject = GetWorld()->SpawnActor<AItemTool_Basic>(HandStatic.EquippedClass, GetWorldSpawnLocation(), GetWorldSpawnRotation(), SpawnInfo);
		if (IsValid(SpawnedObject)) {
			SpawnedObject->Initialize(HandSlotData);
			SpawnedObject->SetActorHiddenInGame(true);
			if (IsValid(GetToolAttachmentComponent())) {
				SpawnedObject->AttachToComponent(GetToolAttachmentComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, GetToolAttachmentSocket());
			}
			else {
				SpawnedObject->AttachToActor(GetOwner(), FAttachmentTransformRules::SnapToTargetIncludingScale);
			}
			HandEquipment = SpawnedObject;
		}
	}

	for (int i = 0; i < GetInventorySize(); i++) {
		UInventorySlotData_Tool* SlotData = Cast<UInventorySlotData_Tool>(GetSlot(i));
		if (IsValid(SlotData)) {
			FItemStaticData_Tool StaticData = UInventorySlotData_Tool::LookupStaticData(SlotData);
			if (FItemStaticData_Tool::IsValid(StaticData)) {
				if (StaticData.EquippedClass && StaticData.EquippedClass->IsChildOf(AItemTool_Basic::StaticClass())) {
					SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					SpawnedObject = GetWorld()->SpawnActor<AItemTool_Basic>(StaticData.EquippedClass, GetWorldSpawnLocation(), GetWorldSpawnRotation(), SpawnInfo);
					if (IsValid(SpawnedObject)) {
						SpawnedObject->Initialize(SlotData);
						SpawnedObject->SetActorHiddenInGame(true);
						if (IsValid(GetToolAttachmentComponent())) {
							SpawnedObject->AttachToComponent(GetToolAttachmentComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, GetToolAttachmentSocket());
						}
						else {
							SpawnedObject->AttachToActor(GetOwner(), FAttachmentTransformRules::SnapToTargetIncludingScale);
						}
						SpawnedEquipment[i] = SpawnedObject;
					}
				}
				else {
					SpawnedEquipment[i] = nullptr;
				}
			}
		}
	}

	if (SpawnedEquipment.IsValidIndex(SelectedIndex) && IsValid(SpawnedEquipment[SelectedIndex])) {
		SpawnedEquipment[SelectedIndex]->OnEquip(GetOwningPlayer());
	} else {
		HandEquipment->OnEquip(GetOwningPlayer());
	}
}

USceneComponent* UHotbarManager::GetToolAttachmentComponent() const
{
	return ToolAttachmentRef;
}

FName UHotbarManager::GetToolAttachmentSocket() const
{
	return ToolAttachmentSocket;
}

void UHotbarManager::SetToolAttachment(USceneComponent* Component, FName Socket)
{
	//Todo: add a way for saving multiple sockets. Aka, let this class handle left hand, right hand, etc.
	ToolAttachmentRef = Component;
	ToolAttachmentSocket = Socket;
	if (IsValid(ToolAttachmentRef)) {
		if (IsValid(HandEquipment)) {
			HandEquipment->AttachToComponent(ToolAttachmentRef, FAttachmentTransformRules::SnapToTargetIncludingScale, Socket);
		}
		for (auto* EquippedItem : SpawnedEquipment) {
			if (IsValid(EquippedItem)) {
				EquippedItem->AttachToComponent(ToolAttachmentRef, FAttachmentTransformRules::SnapToTargetIncludingScale, Socket);
			}
		}
	}
}

int32 UHotbarManager::GetSelectedIndex() const
{
	return SelectedIndex;
}

void UHotbarManager::SetSelectedIndex(const int32 NewIndex)
{
	if (SelectedIndex != NewIndex) {
		int32 WorkingIndex = NewIndex;
		if (NewIndex < 0) {
			WorkingIndex = FMath::Abs(NewIndex);
			WorkingIndex = WorkingIndex % GetInventorySize();
			WorkingIndex = GetInventorySize() - WorkingIndex;
		}
		else if(NewIndex >= GetInventorySize()) {
			WorkingIndex = WorkingIndex % GetInventorySize();
		}
		if (SelectedIndex != WorkingIndex) {
			int32 OldSelectedIndex = SelectedIndex;
			SelectedIndex = WorkingIndex;
			OnHotbarSelectionChanged(SelectedIndex, OldSelectedIndex);
		}
	}
}

void UHotbarManager::SelectItem_Implementation(const int32 ItemIndex)
{
	SetSelectedIndex(ItemIndex);
}

void UHotbarManager::SelectNextItem_Implementation()
{
	SelectItem(GetSelectedIndex() + 1);
}

void UHotbarManager::SelectPreviousItem_Implementation()
{
	SelectItem(GetSelectedIndex() - 1);
}

void UHotbarManager::OnInventoryManagerSlotChanged_Implementation(const int32 TargetSlot)
{
	Super::OnInventoryManagerSlotChanged_Implementation(TargetSlot);
	UInventorySlotData_Tool* SlotData = Cast<UInventorySlotData_Tool>(GetSlot(TargetSlot));
	if (!SpawnedEquipment.IsValidIndex(TargetSlot)) return;
	bool OldDataIsValid = IsValid(SpawnedEquipment[TargetSlot]);
	// If both are is invalid, do nothing.
	if (!IsValid(SlotData) && !OldDataIsValid) return;

	// Get the new data's spawned class, if it exists
	TSubclassOf<AItemTool_Basic> NewDataClass = nullptr;
	if (IsValid(SlotData)) {
		FItemStaticData_Tool StaticData = UInventorySlotData_Tool::LookupStaticData(SlotData);
		if (FItemStaticData_Tool::IsValid(StaticData)) {
			NewDataClass = StaticData.EquippedClass;
		}
	}

	if (OldDataIsValid) {
		// If the spawned data's class is the same as the old data's class, set old data to new data.
		if (NewDataClass == SpawnedEquipment[TargetSlot]->StaticClass()) {
			SpawnedEquipment[TargetSlot]->Initialize(SlotData);
			return;
		}
		// Otherwise, destroy the old data.
		if (TargetSlot == SelectedIndex) {
			SpawnedEquipment[TargetSlot]->OnUnequip(GetOwningPlayer());
		}
		SpawnedEquipment[TargetSlot]->Destroy();
		SpawnedEquipment[TargetSlot] = nullptr;
	}

	//If there is a valid new data class, spawn it. The only way to reach here is if the old data is now invalid.
	if (NewDataClass && NewDataClass->IsChildOf(AItemTool_Basic::StaticClass())) {
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AItemTool_Basic* SpawnedObject = nullptr;
		SpawnedObject = GetWorld()->SpawnActor<AItemTool_Basic>(NewDataClass, GetWorldSpawnLocation(), GetWorldSpawnRotation(), SpawnInfo);
		if (IsValid(SpawnedObject)) {
			SpawnedObject->Initialize(SlotData);
			SpawnedObject->SetActorHiddenInGame(true);
			if (IsValid(GetToolAttachmentComponent())) {
				SpawnedObject->AttachToComponent(GetToolAttachmentComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale, GetToolAttachmentSocket());
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Attaching to Component")));
				UE_LOG(LogTemp, Warning, TEXT("Attaching to Component"));
			}
			else {
				SpawnedObject->AttachToActor(GetOwner(), FAttachmentTransformRules::SnapToTargetIncludingScale);
			}
			SpawnedEquipment[TargetSlot] = SpawnedObject;
		}
	}

	// If the currently-equipped slot has changed, implement those changes.
	if (TargetSlot == SelectedIndex) {
		if (IsValid(SpawnedEquipment[TargetSlot])) {
			if (!OldDataIsValid) {
				// Only runs when the new data is valid, but the old data was not. Aka, the hands were equipped.
				HandEquipment->OnUnequip(GetOwningPlayer());
			}
			SpawnedEquipment[TargetSlot]->OnEquip(GetOwningPlayer());
		}
		else {
			HandEquipment->OnEquip(GetOwningPlayer());
		}
	}
}

void UHotbarManager::OnHotbarSelectionChanged_Implementation(const int32 NewSelectedIndex, const int32 OldSelectedIndex)
{
	if (SpawnedEquipment.IsValidIndex(OldSelectedIndex) && SpawnedEquipment.IsValidIndex(SelectedIndex)) {
		// Only change equipped item if one or both slots are valid.
		if (SpawnedEquipment[OldSelectedIndex] || SpawnedEquipment[SelectedIndex]){
			if (IsValid(SpawnedEquipment[OldSelectedIndex])) {
				SpawnedEquipment[OldSelectedIndex]->OnUnequip(GetOwningPlayer());
			} else {
				HandEquipment->OnUnequip(GetOwningPlayer());
			}
			if (IsValid(SpawnedEquipment[SelectedIndex])) {
				SpawnedEquipment[SelectedIndex]->OnEquip(GetOwningPlayer());
			} else {
				HandEquipment->OnEquip(GetOwningPlayer());
			}
		}
	}
	OnHotbarSelectionChangedDispatcher.Broadcast(SelectedIndex, OldSelectedIndex);
}
