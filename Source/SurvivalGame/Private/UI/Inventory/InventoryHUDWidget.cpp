// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventoryHUDWidget.h"
#include "DEPRECATED/InventoryComponent.h"
#include "Inventory/InventoryManager.h"
#include "Utility/UtilityFunctionLibrary.h"

void UInventoryHUDWidget::UpdateGraphics_Implementation()
{
}

void UInventoryHUDWidget::Rebuild_Implementation()
{
}

void UInventoryHUDWidget::OpenMenu_Implementation()
{
	IsOpen = true;
	UpdateInput();
	//UUtilityFunctionLibrary::PrintDebug("OpenMenu");
}

void UInventoryHUDWidget::CloseMenu_Implementation()
{
	IsOpen = false;
	UpdateInput();
	if (IsValid(InventoryRef)) {
		UUtilityFunctionLibrary::PrintDebug("UI: " + InventoryRef->GetDisplayName() + " has been unlinked");
		InventoryRef->SetLinkedInventory(nullptr);
	}
	//UUtilityFunctionLibrary::PrintDebug("CloseMenu");
}

void UInventoryHUDWidget::ToggleMenu_Implementation()
{
	//UUtilityFunctionLibrary::PrintDebug("ToggleMenu");
	if (IsOpen) {
		Execute_CloseMenu(this);
	}
	else {
		Execute_OpenMenu(this);
	}
}

void UInventoryHUDWidget::UpdateInput_Implementation()
{
	if (IsOpen) {
		FInputModeGameAndUI Mode;
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);
		Mode.SetHideCursorDuringCapture(false);
		APlayerController* controller = GetOwningPlayer<APlayerController>();
		if (controller) {
			controller->SetInputMode(Mode);
			controller->SetShowMouseCursor(true);
		}
	}
	else {
		FInputModeGameOnly Mode;
		APlayerController* controller = GetOwningPlayer<APlayerController>();
		if (controller) {
			controller->SetInputMode(Mode);
			controller->SetShowMouseCursor(false);
		}
	}
}

void UInventoryHUDWidget::SetInventoryRef(UInventoryManager* NewInventoryRef)
{
	if (InventoryRef == NewInventoryRef) { return; }
	if (IsValid(InventoryRef)) {
		InventoryRef->OnLinkedInventoryChangedDispatcher.RemoveDynamic(this, &UInventoryHUDWidget::ToggleMenu);
	}
	InventoryRef = NewInventoryRef;
	if (IsValid(InventoryRef)) {
		InventoryRef->OnLinkedInventoryChangedDispatcher.AddDynamic(this, &UInventoryHUDWidget::ToggleMenu);
	}
	Execute_Rebuild(this);
}
