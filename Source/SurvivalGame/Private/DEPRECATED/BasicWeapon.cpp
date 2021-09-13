// Fill out your copyright notice in the Description page of Project Settings.


#include "DEPRECATED/BasicWeapon.h"
#include "Characters/SurvivalCharacter.h"
//#include "BasicTool.h"
#include "DEPRECATED/InventoryComponent.h"
#include "DEPRECATED/ItemData_SingleInt.h"
#include "DamageType_HealMechanical.h"
#include "Kismet/GameplayStatics.h"

ABasicWeapon::ABasicWeapon()
	:Super()
{
	IsPrimaryActionAutomated = true;
	IsSecondaryActionAutomated = true;
	Damage = 5;
	ToolRange = 100.f;
	AmmoCount_MAX = 1;
}

inline FHitResult ABasicWeapon::GetLastHitResult() const { return LastHitResult; }

bool ABasicWeapon::UpdateLastHitResult()
{
	if (IsValid(GetOwningCharacter())) {
		LastHitResult = GetOwningCharacter()->GetLastLookTrace();
		return true;
	}
	return false;
}

void ABasicWeapon::SetupPlayerInputComponent()
{
	Super::SetupPlayerInputComponent();
	InputComponent->BindAction("Reload", IE_Pressed,  this, &ABasicWeapon::OnReloadAction_Start);
	InputComponent->BindAction("Reload", IE_Released, this, &ABasicWeapon::OnReloadAction_End);
}

bool ABasicWeapon::CanReloadAction_Implementation() const
{
	return true;
}

void ABasicWeapon::OnReloadAction_Implementation()
{
}

void ABasicWeapon::OnReloadAction_Start()
{
	if (!CanReloadAction()) { return; }
	OnReloadAction();
}

void ABasicWeapon::OnReloadAction_End()
{
}

void ABasicWeapon::ConsumeAmmo(int32 AmountToConsume)
{
	SetCurrentAmmoCount(GetCurrentAmmoCount() - AmountToConsume);
	InventoryRef->OnInventorySlotChangedDispatcher.Broadcast(InventorySlotIndex);
}

void ABasicWeapon::SetCurrentAmmoCount(int32 NewAmmoCount)
{
	if (IsValid(WeaponDataRef)) {
		WeaponDataRef->SetStat(FMath::Clamp(NewAmmoCount, 0, AmmoCount_MAX));
	}
}

int32 ABasicWeapon::GetCurrentAmmoCount() const
{
	if (IsValid(WeaponDataRef)) {
		return WeaponDataRef->GetStat();
	}
	return 0;
}

void ABasicWeapon::OnPrimaryAction_Implementation() { if (!CanPrimaryAction()) { OnPrimaryAction_End(); return; } }
void ABasicWeapon::OnSecondaryAction_Implementation() { if (!CanSecondaryAction()) { OnSecondaryAction_End(); return; } }
void ABasicWeapon::OnTertiaryAction_Implementation() { if (!CanTertiaryAction()) { OnTertiaryAction_End(); return; } }

void ABasicWeapon::Initialize_Implementation(const int32 SlotIndex, UBasicItemData * NewItemInstanceData)
{
	Super::Initialize_Implementation(SlotIndex, NewItemInstanceData);
	if ( IsValid(NewItemInstanceData) ) {
		WeaponDataRef = Cast<UItemData_SingleInt>(NewItemInstanceData);
		WeaponDataRef->SetStatMAX(AmmoCount_MAX);
	}
}
