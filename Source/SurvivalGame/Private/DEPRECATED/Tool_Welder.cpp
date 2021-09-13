// Fill out your copyright notice in the Description page of Project Settings.


#include "DEPRECATED/Tool_Welder.h"
#include "Characters/SurvivalCharacter.h"
#include "DEPRECATED/InventoryComponent.h"
#include "DEPRECATED/BasicWeaponData.h"
#include "DamageType_HealMechanical.h"
#include "Kismet/GameplayStatics.h" 	
#include "Engine/DataTable.h"
#include "Engine/CompositeDataTable.h"
#include "Utility/UtilityFunctionLibrary.h"


ATool_Welder::ATool_Welder()
	:Super()
{
	IsPrimaryActionAutomated = true;
	IsSecondaryActionAutomated = true;
	Damage = 5.f;
	//AmmoCount = 0.f;
	AmmoCount_MAX = 100.f;
	AmmoData = FItemSlot();
	//AmmoData.StaticDataHandle = FDataTableRowHandle();

	// Finding data table objects can cause the editor to crash on load. Probably starts when the table is edited.
	/*
	const ConstructorHelpers::FObjectFinder<UCompositeDataTable> findObject(TEXT("CompositeDataTable'/Game/SurvivalGame/Dev/Data/devItemTable.devItemTable'"));//"/Game/SurvivalGame/Dev/Data/devItemTable"));//
	if (findObject.Succeeded()) {
		AmmoData.StaticDataHandle.DataTable = findObject.Object;
	}
	AmmoData.StaticDataHandle.RowName = "devAmmoBattery";
	//*/
	AmmoData.Amount = 1.f;
}

bool ATool_Welder::CanPrimaryAction_Implementation() const
{
	return GetCurrentAmmoCount() - AmmoData.Amount >= 0;
}

bool ATool_Welder::CanSecondaryAction_Implementation() const
{
	return GetCurrentAmmoCount() - AmmoData.Amount >= 0;
}

bool ATool_Welder::CanReloadAction_Implementation() const
{
	if (GetCurrentAmmoCount() + 20 > AmmoCount_MAX) {
		return false;
	}
	if (IsValid(InventoryRef)) {
		if (InventoryRef->CanRemoveFromInventory(AmmoData)) {
			//UUtilityFunctionLibrary::PrintDebug("Can Remove...");
			return true;
		}
	}
	return false;
}

void ATool_Welder::OnReloadAction_Implementation()
{
	if (IsValid(InventoryRef)) {
		FItemSlot Remainder;
		if (InventoryRef->RemoveFromInventory(AmmoData, Remainder)) { return; }
		ConsumeAmmo(-20);
		//UUtilityFunctionLibrary::PrintDebug("Reloaded: " + FString::FromInt(GetCurrentAmmoCount()));
		Super::OnReloadAction_Implementation();
	}
}

void ATool_Welder::OnPrimaryAction_Implementation()
{
	UpdateLastHitResult();
	if (GetLastHitResult().bBlockingHit) {
		if (GetLastHitResult().Distance > ToolRange) { return; }
		AActor* HitActor = GetLastHitResult().GetActor();
		if (IsValid(HitActor)) {
			UGameplayStatics::ApplyDamage(HitActor, Damage, GetOwningCharacter()->GetController(), GetOwningCharacter(), UDamageType_HealMechanical::StaticClass());
		}
	}
	ConsumeAmmo(AmmoData.Amount);
	Super::OnPrimaryAction_Implementation();
}

void ATool_Welder::OnSecondaryAction_Implementation()
{
	UpdateLastHitResult();
	if (GetLastHitResult().bBlockingHit) {
		if (GetLastHitResult().Distance > ToolRange) { return; }
		AActor* HitActor = GetLastHitResult().GetActor();
		if (IsValid(HitActor)) {
			UGameplayStatics::ApplyDamage(HitActor, Damage, GetOwningCharacter()->GetController(), GetOwningCharacter(), UDamageType::StaticClass());
		}
	}
	ConsumeAmmo(AmmoData.Amount);
	Super::OnSecondaryAction_Implementation();
}

//void ATool_Welder::Initialize_Implementation(UBasicItemData* NewItemInstanceData)
//{
//	Super::Initialize_Implementation(NewItemInstanceData);
//}
