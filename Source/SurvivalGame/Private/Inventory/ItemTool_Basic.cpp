// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ItemTool_Basic.h"
#include "Characters/SurvivalCharacter.h"
#include "Inventory/InventoryManager.h"
#include "Inventory/InventorySlotData_Tool.h"
#include "Utility/UtilityFunctionLibrary.h"

#define Debug(str) UUtilityFunctionLibrary::PrintDebug(str);

// Sets default values
AItemTool_Basic::AItemTool_Basic()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	IsEquipped = false;

	ToolName = "Hands";
	AttachmentTransform = FTransform();
	IsPrimaryActionAutomated = false;
	PrimaryActionRate = 0.1f;
	IsSecondaryActionAutomated = false;
	SecondaryActionRate = 0.1f;
	IsTertiaryActionAutomated = false;
	TertiaryActionRate = 0.1f;
	InventorySlotIndex = 0;
}

inline void AItemTool_Basic::SetIsEquipped(const bool NewIsEquipped) { IsEquipped = NewIsEquipped; }
inline bool AItemTool_Basic::GetIsEquipped() const { return IsEquipped; }
inline FTransform AItemTool_Basic::GetAttachmentTransform() const { return AttachmentTransform; }
inline ASurvivalCharacter* AItemTool_Basic::GetOwningCharacter() const { return OwningCharacterRef; }

inline bool AItemTool_Basic::CanPrimaryAction_Implementation() const { return true; }
inline bool AItemTool_Basic::CanSecondaryAction_Implementation() const { return true; }
inline bool AItemTool_Basic::CanTertiaryAction_Implementation() const { return true; }

void AItemTool_Basic::SetupPlayerInputComponent()
{
	InputComponent->BindAction("PrimaryAction", IE_Pressed, this, &AItemTool_Basic::OnPrimaryAction_Start);
	InputComponent->BindAction("PrimaryAction", IE_Released, this, &AItemTool_Basic::OnPrimaryAction_End);
	InputComponent->BindAction("SecondaryAction", IE_Pressed, this, &AItemTool_Basic::OnSecondaryAction_Start);
	InputComponent->BindAction("SecondaryAction", IE_Released, this, &AItemTool_Basic::OnSecondaryAction_End);
	InputComponent->BindAction("TertiaryAction", IE_Pressed, this, &AItemTool_Basic::OnTertiaryAction_Start);
	InputComponent->BindAction("TertiaryAction", IE_Released, this, &AItemTool_Basic::OnTertiaryAction_End);
}

void AItemTool_Basic::SetOwningCharacter(ASurvivalCharacter* NewOwningCharacterRef)
{
	OwningCharacterRef = NewOwningCharacterRef;
	if (IsValid(OwningCharacterRef)) {
		InventoryRef = OwningCharacterRef->FindComponentByClass<UInventoryManager>();
	}
}

void AItemTool_Basic::OnPrimaryAction_Implementation()
{
	Debug("Jazz hands!");
	//UUtilityFunctionLibrary::PrintDebug("Jazz hands!");
}

void AItemTool_Basic::OnPrimaryAction_Start()
{
	if (!CanPrimaryAction()) { return; }
	OnPrimaryAction();
	if (IsPrimaryActionAutomated) {
		GetWorld()->GetTimerManager().SetTimer(PrimaryTimerHandle, this, &AItemTool_Basic::OnPrimaryAction, PrimaryActionRate, true);
	}
}

void AItemTool_Basic::OnPrimaryAction_End()
{
	if (PrimaryTimerHandle.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(PrimaryTimerHandle);
	}
}

void AItemTool_Basic::OnSecondaryAction_Implementation()
{
	Debug("Second jazz hands!");
	//UUtilityFunctionLibrary::PrintDebug("Second jazz hands!");
}

void AItemTool_Basic::OnSecondaryAction_Start()
{
	if (!CanSecondaryAction()) { return; }
	OnSecondaryAction();
	if (IsSecondaryActionAutomated) {
		GetWorld()->GetTimerManager().SetTimer(SecondaryTimerHandle, this, &AItemTool_Basic::OnSecondaryAction, SecondaryActionRate, true);
	}
}

void AItemTool_Basic::OnSecondaryAction_End()
{
	if (SecondaryTimerHandle.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(SecondaryTimerHandle);
	}
}


void AItemTool_Basic::OnTertiaryAction_Implementation()
{
	UUtilityFunctionLibrary::PrintDebug("Third jazz hands!");
}

void AItemTool_Basic::OnTertiaryAction_Start()
{
	if (!CanTertiaryAction()) { return; }
	OnTertiaryAction();
	if (IsTertiaryActionAutomated) {
		GetWorld()->GetTimerManager().SetTimer(TertiaryTimerHandle, this, &AItemTool_Basic::OnTertiaryAction, TertiaryActionRate, true);
	}
}

void AItemTool_Basic::OnTertiaryAction_End()
{
	if (TertiaryTimerHandle.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(TertiaryTimerHandle);
	}
}

void AItemTool_Basic::OnEquip_Implementation(APlayerController* ControllerRef_)
{
	SetActorHiddenInGame(false);
	if (!GetIsEquipped()) {
		IsEquipped = true;
		EnableInput(ControllerRef_);
		SetupPlayerInputComponent();
		SetOwningCharacter(Cast<ASurvivalCharacter>(ControllerRef_->GetPawn()));
	}
}

void AItemTool_Basic::OnUnequip_Implementation(APlayerController* ControllerRef_, const bool ShouldDestroyActor)
{
	SetActorHiddenInGame(true);
	if (GetIsEquipped()) {
		IsEquipped = false;
		DisableInput(ControllerRef_);
	}
	if (ShouldDestroyActor) {
		Destroy();
	}
}

void AItemTool_Basic::Initialize_Implementation(const int32 SlotIndex, const UInventorySlotData_Tool* ItemInstanceData_)
{
	InventorySlotIndex = SlotIndex;
}
