// Fill out your copyright notice in the Description page of Project Settings.


#include "DEPRECATED/HotbarHandlerComponent.h"
//#include "InventoryTypes.h"
#include "DEPRECATED/InventoryComponent.h"
#include "DEPRECATED/BasicTool.h"
#include "Utility/UtilityFunctionLibrary.h"

// Sets default values for this component's properties
UHotbarHandlerComponent::UHotbarHandlerComponent()
	:Super()
{
	SelectedIndex = 0;
	AttachmentSocket = NAME_None;
}

// Called when the game starts
void UHotbarHandlerComponent::BeginPlay()
{
	Super::BeginPlay();
	SpawnedToolRefs.SetNum(NumSlotsToReserve);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	HandsToolRef = GetWorld()->SpawnActor<ABasicTool>(ABasicTool::StaticClass(), GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation(), SpawnInfo);
	if (HandsToolRef && AttachmentRef) {
		HandsToolRef->AttachToComponent(AttachmentRef, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false), AttachmentSocket);
		HandsToolRef->SetActorRelativeTransform(HandsToolRef->GetAttachmentTransform());
	}
	EquipItem();
	if (IsValid(InventoryRef)) {
		//InventoryRef->OnInventoryChangedDispatcher.AddDynamic(this, &UHotbarHandlerComponent::OnInventoryUpdated);
		InventoryRef->OnInventorySlotChangedDispatcher.AddDynamic(this, &UHotbarHandlerComponent::OnInventoryUpdated);
	}
}

inline int32 UHotbarHandlerComponent::GetSelectedIndex() const { return SelectedIndex; }

inline void UHotbarHandlerComponent::SetAttachment(USceneComponent* Component, FName Socket) { AttachmentRef = Component; AttachmentSocket = Socket; }

void UHotbarHandlerComponent::SetSelectedItem(int32 NewIndex)
{
	if (SelectedIndex == NewIndex) { return; }
	UnequipItem();
	SelectedIndex = FMath::Clamp(NewIndex, 0, NumSlotsToReserve - 1);
	EquipItem();
	//OnHotbarSelectionChangedDispatcher.Broadcast(SelectedIndex);
}

void UHotbarHandlerComponent::SelectNextItem()
{
	if (SelectedIndex + 1 >= NumSlotsToReserve) {
		SetSelectedItem(0);
	}
	else {
		SetSelectedItem(SelectedIndex + 1);
	}
}

void UHotbarHandlerComponent::SelectPreviousItem()
{
	if (SelectedIndex - 1 < 0) {
		SetSelectedItem(NumSlotsToReserve - 1);
	}
	else {
		SetSelectedItem(SelectedIndex - 1);
	}
}

void UHotbarHandlerComponent::EquipItem()
{
	if (!IsValid(SpawnedToolRefs[SelectedIndex])) {
		FItemStaticData StaticData;
		// If there is an item in the selected slot, create an actor based on the static data's stored class
		int32 InventorySlotIndex;
		if (GetReservedItemSlot_WithStaticData(SelectedIndex, InventorySlotIndex, SelectedItemData, StaticData)) {
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			if (StaticData.EquippedClass && StaticData.EquippedClass != ABasicTool::StaticClass()) {
				SpawnedToolRefs[SelectedIndex] = GetWorld()->SpawnActor<ABasicTool>(StaticData.EquippedClass, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation(), SpawnInfo);
				if (IsValid(SpawnedToolRefs[SelectedIndex]) && IsValid(AttachmentRef)) {
					SpawnedToolRefs[SelectedIndex]->AttachToComponent(AttachmentRef, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false), AttachmentSocket);
					SpawnedToolRefs[SelectedIndex]->SetActorRelativeTransform(SpawnedToolRefs[SelectedIndex]->GetAttachmentTransform());
					//if (IsValid(SelectedItemData.InstancedDataRef)) {
						SpawnedToolRefs[SelectedIndex]->Initialize(InventorySlotIndex, SelectedItemData.InstancedDataRef);
					//}
					//else {
						SpawnedToolRefs[SelectedIndex]->Initialize(InventorySlotIndex, nullptr);
					//}
				}
			}
		}
	}
	if (IsValid(SpawnedToolRefs[SelectedIndex])) {
		SpawnedToolRefs[SelectedIndex]->OnEquip(GetOwningPlayer());
	}
	else {
		HandsToolRef->OnEquip(GetOwningPlayer());
	}
}

void UHotbarHandlerComponent::UnequipItem(bool ShouldDestroyActor)
{
	if (IsValid(SpawnedToolRefs[SelectedIndex])) {
		SpawnedToolRefs[SelectedIndex]->OnUnequip(GetOwningPlayer(), ShouldDestroyActor);
	}
	
	HandsToolRef->OnEquip(GetOwningPlayer());
}

void UHotbarHandlerComponent::OnInventoryUpdated(const int32 TargetSlot)
{
	FItemSlot ItemData;
	if (GetReservedItemSlot(SelectedIndex, ItemData) ) {
		if (ItemData != SelectedItemData) {
			UnequipItem(true);
			EquipItem();
		}
	}
	else if( FItemSlot::IsValid(SelectedItemData) ){
		UnequipItem(true);
		EquipItem();
	}
}

