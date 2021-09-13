// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/HotbarManager.h"

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
	ToolAttachmentRef = Component;
	ToolAttachmentSocket = Socket;
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
			OnHotbarSelectionChangedDispatcher.Broadcast(SelectedIndex, OldSelectedIndex);
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
