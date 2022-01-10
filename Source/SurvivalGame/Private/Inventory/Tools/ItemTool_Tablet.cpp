// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Tools/ItemTool_Tablet.h"

AItemTool_Tablet::AItemTool_Tablet()
	:Super()
{

}

bool AItemTool_Tablet::CanTertiaryAction_Implementation() const
{
	return false;
}
