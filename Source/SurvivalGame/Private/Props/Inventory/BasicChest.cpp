// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/Inventory/BasicChest.h"
#include "Components/StaticMeshComponent.h"
#include "Inventory/InventoryManager.h"
#include "Utility/UtilityFunctionLibrary.h"

// Sets default values
ABasicChest::ABasicChest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InventoryManager = CreateDefaultSubobject<UInventoryManager>(TEXT("InventoryManager"));
	InventoryManager->SetDisplayName("Chest");
	InventoryManager->SetInventorySize(4);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> meshObject(TEXT("/Game/SurvivalGame/Dev/Meshes/SM_DevChest_Box"));
	if (meshObject.Succeeded()) {
		StaticMesh->SetStaticMesh(meshObject.Object);
	}
	StaticMesh->SetupAttachment(GetRootComponent());
	StaticMesh->bEditableWhenInherited = true;
	StaticMesh->bSelectable = true;
	//RootComponent = StaticMesh;
}


bool ABasicChest::CanInteract_Implementation(const FInteractionInfo Info) const
{
	if (IsValid(Info.ControllerRef) == false) { return false; }
	if (IsValid(InteractingControllerRef)) {
		if (Info.ControllerRef == InteractingControllerRef) {
			return true;
		}
	}
	else {
		return true;
	}
	return false;
}

void ABasicChest::BeginInteraction_Implementation(const FInteractionInfo Info)
{
	if (Info.ControllerRef && IsValid(Info.ControllerRef) == false) { return; }
	if (InteractingControllerRef && IsValid(InteractingControllerRef)) {
		if (InteractingControllerRef != Info.ControllerRef) {
			return;
		}
	}
	InteractingControllerRef = Info.ControllerRef;
	UInventoryManager* InventoryRef = Info.PawnRef->FindComponentByClass<UInventoryManager>();
	if (IsValid(InventoryRef)) {
		InventoryRef->SetLinkedInventory(InventoryManager);
		InventoryManager->SetLinkedInventory(InventoryRef);
		UUtilityFunctionLibrary::PrintDebug("Chest: " + InventoryManager->GetDisplayName() + " has been linked to " + InventoryRef->GetDisplayName());
		if (IsValid(InventoryManager->GetLinkedInventory())) {
			InventoryManager->GetLinkedInventory()->OnLinkedInventoryChangedDispatcher.AddUniqueDynamic(this, &ABasicChest::ForceCancelInteraction);
		}
	}
}

void ABasicChest::CancelInteraction_Implementation(const FInteractionInfo Info)
{
	if (Info.ControllerRef && IsValid(Info.ControllerRef) == false) { return; }
	if (InteractingControllerRef && IsValid(InteractingControllerRef)) {
		if (InteractingControllerRef != Info.ControllerRef) {
			return;
		}
	}
	InteractingControllerRef = nullptr;
	if (IsValid(InventoryManager->GetLinkedInventory())) {
		if (InventoryManager->GetLinkedInventory()->OnLinkedInventoryChangedDispatcher.IsAlreadyBound(this, &ABasicChest::ForceCancelInteraction)) {
			InventoryManager->GetLinkedInventory()->OnLinkedInventoryChangedDispatcher.RemoveDynamic(this, &ABasicChest::ForceCancelInteraction);
		}
		UUtilityFunctionLibrary::PrintDebug("Chest: " + InventoryManager->GetDisplayName() + " has been unlinked");
		InventoryManager->SetLinkedInventory(nullptr);
	}
}

void ABasicChest::ForceCancelInteraction()
{
	FInteractionInfo FakeInteractionInfo = FInteractionInfo();
	if (IsValid(InteractingControllerRef)) {
		FakeInteractionInfo.ControllerRef = InteractingControllerRef;
		FakeInteractionInfo.PawnRef = InteractingControllerRef->GetPawn();
	}
	else {
		if (InventoryManager->GetLinkedInventory()->OnLinkedInventoryChangedDispatcher.IsAlreadyBound(this, &ABasicChest::ForceCancelInteraction)) {
			InventoryManager->GetLinkedInventory()->OnLinkedInventoryChangedDispatcher.RemoveDynamic(this, &ABasicChest::ForceCancelInteraction);
		}
	}
	Execute_CancelInteraction(this, FakeInteractionInfo);
}
