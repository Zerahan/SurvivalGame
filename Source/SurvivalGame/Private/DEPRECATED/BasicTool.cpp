// Fill out your copyright notice in the Description page of Project Settings.


#include "DEPRECATED/BasicTool.h"
#include "DEPRECATED/BasicItemData.h"
#include "Characters/SurvivalCharacter.h"
#include "DEPRECATED/InventoryComponent.h"
#include "Utility/UtilityFunctionLibrary.h"

#define Debug(str) UUtilityFunctionLibrary::PrintDebug(str);

// Sets default values
ABasicTool::ABasicTool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	IsEquipped = false;

	ToolName					= "Hands";
	AttachmentTransform			= FTransform();
	IsPrimaryActionAutomated	= false;
	PrimaryActionRate			= 0.1f;
	IsSecondaryActionAutomated	= false;
	SecondaryActionRate			= 0.1f;
	IsTertiaryActionAutomated	= false;
	TertiaryActionRate			= 0.1f;
	InventorySlotIndex			= 0;
}

inline void ABasicTool::SetIsEquipped(const bool NewIsEquipped)		{ IsEquipped = NewIsEquipped; }
inline bool ABasicTool::GetIsEquipped() const						{ return IsEquipped; }
inline FTransform ABasicTool::GetAttachmentTransform() const		{ return AttachmentTransform; }
inline ASurvivalCharacter* ABasicTool::GetOwningCharacter() const	{ return OwningCharacterRef; }

inline bool ABasicTool::CanPrimaryAction_Implementation() const		{ return true; }
inline bool ABasicTool::CanSecondaryAction_Implementation() const	{ return true; }
inline bool ABasicTool::CanTertiaryAction_Implementation() const	{ return true; }

void ABasicTool::SetupPlayerInputComponent()
{
	InputComponent->BindAction("PrimaryAction",   IE_Pressed,  this, &ABasicTool::OnPrimaryAction_Start);
	InputComponent->BindAction("PrimaryAction",   IE_Released, this, &ABasicTool::OnPrimaryAction_End);
	InputComponent->BindAction("SecondaryAction", IE_Pressed,  this, &ABasicTool::OnSecondaryAction_Start);
	InputComponent->BindAction("SecondaryAction", IE_Released, this, &ABasicTool::OnSecondaryAction_End);
	InputComponent->BindAction("TertiaryAction",  IE_Pressed,  this, &ABasicTool::OnTertiaryAction_Start);
	InputComponent->BindAction("TertiaryAction",  IE_Released, this, &ABasicTool::OnTertiaryAction_End);
}

void ABasicTool::SetOwningCharacter(ASurvivalCharacter * NewOwningCharacterRef)
{
	OwningCharacterRef = NewOwningCharacterRef;
	if (IsValid(OwningCharacterRef)) {
		InventoryRef = OwningCharacterRef->FindComponentByClass<UInventoryComponent>();
	}
}

void ABasicTool::OnPrimaryAction_Implementation()
{
	Debug("Jazz hands!");
	//UUtilityFunctionLibrary::PrintDebug("Jazz hands!");
}

void ABasicTool::OnPrimaryAction_Start()
{
	if (!CanPrimaryAction()) { return; }
	OnPrimaryAction();
	if (IsPrimaryActionAutomated) {
		GetWorld()->GetTimerManager().SetTimer(PrimaryTimerHandle, this, &ABasicTool::OnPrimaryAction, PrimaryActionRate, true);
	}
}

void ABasicTool::OnPrimaryAction_End()
{
	if (PrimaryTimerHandle.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(PrimaryTimerHandle);
	}
}


void ABasicTool::OnSecondaryAction_Implementation()
{
	Debug("Second jazz hands!");
	//UUtilityFunctionLibrary::PrintDebug("Second jazz hands!");
}

void ABasicTool::OnSecondaryAction_Start()
{
	if (!CanSecondaryAction()) { return; }
	OnSecondaryAction();
	if (IsSecondaryActionAutomated) {
		GetWorld()->GetTimerManager().SetTimer(SecondaryTimerHandle, this, &ABasicTool::OnSecondaryAction, SecondaryActionRate, true);
	}
}

void ABasicTool::OnSecondaryAction_End()
{
	if (SecondaryTimerHandle.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(SecondaryTimerHandle);
	}
}


void ABasicTool::OnTertiaryAction_Implementation()
{
	UUtilityFunctionLibrary::PrintDebug("Third jazz hands!");
}

void ABasicTool::OnTertiaryAction_Start()
{
	if (!CanTertiaryAction()) { return; }
	OnTertiaryAction();
	if (IsTertiaryActionAutomated) {
		GetWorld()->GetTimerManager().SetTimer(TertiaryTimerHandle, this, &ABasicTool::OnTertiaryAction, TertiaryActionRate, true);
	}
}

void ABasicTool::OnTertiaryAction_End()
{
	if (TertiaryTimerHandle.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(TertiaryTimerHandle);
	}
}

void ABasicTool::OnEquip_Implementation(APlayerController* ControllerRef_)
{
	SetActorHiddenInGame(false);
	if (!GetIsEquipped()) {
		IsEquipped = true;
		EnableInput(ControllerRef_);
		SetupPlayerInputComponent();
		SetOwningCharacter(Cast<ASurvivalCharacter>(ControllerRef_->GetPawn()));
	}
}

void ABasicTool::OnUnequip_Implementation(APlayerController* ControllerRef_, const bool ShouldDestroyActor)
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

void ABasicTool::Initialize_Implementation(const int32 SlotIndex, UBasicItemData * ItemInstanceData_)
{
	InventorySlotIndex = SlotIndex;
}
