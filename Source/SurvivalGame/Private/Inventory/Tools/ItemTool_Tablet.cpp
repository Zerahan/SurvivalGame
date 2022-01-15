// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Tools/ItemTool_Tablet.h"

AItemTool_Tablet::AItemTool_Tablet()
	:Super()
{
	IsTabletOpen = false;
	ToolName = "Tablet";
	HoldAnimation = EHoldingAnimation::Carry1H;
}

bool AItemTool_Tablet::CanPrimaryAction_Implementation() const
{
	return true;
}

bool AItemTool_Tablet::CanSecondaryAction_Implementation() const
{
	return !IsTabletOpen;
}

bool AItemTool_Tablet::CanTertiaryAction_Implementation() const
{
	return false;
}

void AItemTool_Tablet::OnPrimaryAction_Implementation()
{
}

void AItemTool_Tablet::OnSecondaryAction_Implementation()
{
}
