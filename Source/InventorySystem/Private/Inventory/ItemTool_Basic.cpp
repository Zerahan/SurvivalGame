// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ItemTool_Basic.h"
#include "Inventory/InventoryManager.h"
#include "Inventory/InventorySlotData_Tool.h"

// Sets default values
AItemTool_Basic::AItemTool_Basic()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	IsEquipped					= false;

	ToolName					= "Hands";
	HoldAnimation				= EHoldingAnimation::Hands;
	AttachmentTransform			= FTransform();
	IsPrimaryActionAutomated	= false;
	PrimaryActionRate			= 0.1f;
	IsSecondaryActionAutomated	= false;
	SecondaryActionRate			= 0.1f;
	IsTertiaryActionAutomated	= false;
	TertiaryActionRate			= 0.1f;
}

inline void AItemTool_Basic::SetIsEquipped(const bool NewIsEquipped) { IsEquipped = NewIsEquipped; }
inline bool AItemTool_Basic::GetIsEquipped() const { return IsEquipped; }
inline EHoldingAnimation AItemTool_Basic::GetHoldType() const { return HoldAnimation; }
inline FTransform AItemTool_Basic::GetAttachmentTransform() const { return AttachmentTransform; }

inline bool AItemTool_Basic::CanPrimaryAction_Implementation() const { return true; }
inline bool AItemTool_Basic::CanSecondaryAction_Implementation() const { return true; }
inline bool AItemTool_Basic::CanTertiaryAction_Implementation() const { return true; }

void AItemTool_Basic::SetupPlayerInputComponent()
{
	InputComponent->BindAction("PrimaryAction",   IE_Pressed,  this, &AItemTool_Basic::OnPrimaryAction_Start);
	InputComponent->BindAction("PrimaryAction",   IE_Released, this, &AItemTool_Basic::OnPrimaryAction_End);
	InputComponent->BindAction("SecondaryAction", IE_Pressed,  this, &AItemTool_Basic::OnSecondaryAction_Start);
	InputComponent->BindAction("SecondaryAction", IE_Released, this, &AItemTool_Basic::OnSecondaryAction_End);
	InputComponent->BindAction("TertiaryAction",  IE_Pressed,  this, &AItemTool_Basic::OnTertiaryAction_Start);
	InputComponent->BindAction("TertiaryAction",  IE_Released, this, &AItemTool_Basic::OnTertiaryAction_End);
}

void AItemTool_Basic::SetOwnerInfo(APawn* NewOwnerRef)
{
	if (IsValid(NewOwnerRef)) {
		InventoryRef = NewOwnerRef->FindComponentByClass<UInventoryManager>();
	}
}

void AItemTool_Basic::OnPrimaryAction_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s: Jazz hands!"), *GetName()));
	UE_LOG(LogTemp, Warning, TEXT("%s: Jazz hands!"), *GetName()); 
	//Debug("Jazz hands!");
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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s: Second jazz hands!"), *GetName()));
	UE_LOG(LogTemp, Warning, TEXT("%s: Second jazz hands!"), *GetName());
	//Debug("Second jazz hands!");
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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s: Third jazz hands!"), *GetName()));
	UE_LOG(LogTemp, Warning, TEXT("%s: Third jazz hands!"), *GetName());
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
		SetOwnerInfo(ControllerRef_->GetPawn());
	}
}

void AItemTool_Basic::OnUnequip_Implementation(APlayerController* ControllerRef_, const bool ShouldDestroyActor)
{
	SetActorHiddenInGame(true);
	if (GetIsEquipped()) {
		IsEquipped = false;
		DisableInput(ControllerRef_);
		InputComponent->ClearActionBindings();
	}
	if (ShouldDestroyActor) {
		Destroy();
	}
}

void AItemTool_Basic::Initialize_Implementation(UInventorySlotData_Tool* ItemInstanceData_)
{
	ToolDataRef			= ItemInstanceData_;
	ToolStaticDataRef	= UInventorySlotData_Tool::LookupStaticData(ToolDataRef);
}
