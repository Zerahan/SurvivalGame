// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Tools/ItemTool_Welder.h"
#include "Utility/UtilityFunctionLibrary.h"

#define Debug(str) UUtilityFunctionLibrary::PrintDebug(str);

AItemTool_Welder::AItemTool_Welder()
	:Super()
{

}

//void AItemTool_Welder::OnPrimaryAction_Implementation()
//{
//	Debug(GetName() + ": Welding!");
//}

bool AItemTool_Welder::CanSecondaryAction_Implementation() const
{
	return false;
}

bool AItemTool_Welder::CanTertiaryAction_Implementation() const
{
	return false;
}
