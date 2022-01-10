// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/DroppedItem_Basic.h"
#include "GameFramework/Actor.h"
#include "Inventory/InventorySlotData_Basic.h"
#include "Inventory/InventoryManager.h"
#include "Interfaces/InteractionInterface.h"

// Sets default values
ADroppedItem_Basic::ADroppedItem_Basic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADroppedItem_Basic::BeginPlay()
{
	Super::BeginPlay();
	UpdateGraphics();
}

// Called every frame
void ADroppedItem_Basic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UInventorySlotData_Basic* ADroppedItem_Basic::GetItemSlot() const { return ItemData; }

void ADroppedItem_Basic::SetItemSlot(UInventorySlotData_Basic* NewItemData)
{
	ItemData = NewItemData;
	ItemData->Rename(nullptr, this);
	UpdateGraphics();
}

bool ADroppedItem_Basic::CanInteract_Implementation(const FInteractionInfo Info) const
{
	if (IsPendingKillOrUnreachable()) { return false; }
	if (!IsValid(Info.ControllerRef)) { return false; }
	if (!IsValid(Info.PawnRef)) { return false; }
	if (IsValid(InteractingControllerRef)) {
		if (Info.ControllerRef == InteractingControllerRef) {
			return true;
		}
	}
	else {
		return true;
	}
	return false;
}

void ADroppedItem_Basic::BeginInteraction_Implementation(const FInteractionInfo Info)
{
	if (!Execute_CanInteract(this,Info)) { return; }
	InteractingControllerRef = Info.ControllerRef;
	UInventoryManager* InventoryRef = Info.PawnRef->FindComponentByClass<UInventoryManager>();
	if (!IsValid(InventoryRef)) { return; }
	if (!IsValid(ItemData)) { return; }
	int32 Remainder = -1;
	InventoryRef->MergeToInventory(ItemData, Remainder);
	if (!ItemData){
		Destroy();
		return;
	}
	if(IsValid(ItemData) && ItemData->GetOuter() != this) {
		Destroy();
		return;
	}
}

void ADroppedItem_Basic::EndInteraction_Implementation(const FInteractionInfo Info)
{
	InteractingControllerRef = nullptr;
}

void ADroppedItem_Basic::UpdateGraphics_Implementation()
{
	OnUpdateGraphicsDispatcher.Broadcast();
}

/*
// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/DroppedItem_Basic.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "BasicItemSlotWidget.h"
#include "InventoryComponent.h"

// Sets default values
ADroppedItem_Basic::ADroppedItem_Basic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DropMesh"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> meshObject(TEXT("/Engine/VREditor/BasicMeshes/SM_Cube_01"));
	if (meshObject.Succeeded()) {
		RootStaticMeshComponent->SetStaticMesh(meshObject.Object);
	}
	RootStaticMeshComponent->SetSimulatePhysics(true);
	RootStaticMeshComponent->bEditableWhenInherited = true;
	RootStaticMeshComponent->bSelectable = true;
	RootComponent = RootStaticMeshComponent;
	
	ItemDataDisplayComponent1 = CreateDefaultSubobject<UWidgetComponent>(TEXT("Display1"));
	ItemDataDisplayComponent2 = CreateDefaultSubobject<UWidgetComponent>(TEXT("Display2"));
	ItemDataDisplayComponent1->SetupAttachment(RootComponent);
	ItemDataDisplayComponent2->SetupAttachment(RootComponent);
	ItemDataDisplayComponent1->SetRelativeLocationAndRotation(FVector(0, 25, 0), FRotator(0, 90.f, 0));
	ItemDataDisplayComponent2->SetRelativeLocationAndRotation(FVector(0, -25, 0), FRotator(0, -90.f, 0));
	ItemDataDisplayComponent1->SetDrawSize(FVector2D(80.f, 80.f));
	ItemDataDisplayComponent2->SetDrawSize(FVector2D(80.f, 80.f));
	ItemDataDisplayComponent1->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	ItemDataDisplayComponent2->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));

	ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/GameCore/Dev/UI/InventoryDroppedItemSlot"));
	if (WidgetClass.Succeeded()) {
		ItemDataDisplayComponent1->SetWidgetClass(WidgetClass.Class);
		ItemDataDisplayComponent2->SetWidgetClass(WidgetClass.Class);
	}
}

// Called when the game starts or when spawned
void ADroppedItem_Basic::BeginPlay()
{
	Super::BeginPlay();
	UpdateGraphics();
}

// Called every frame
void ADroppedItem_Basic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ADroppedItem_Basic::SetItemData(const FItemSlot& NewItemData)
{
	ItemData = NewItemData;
	UpdateGraphics();
	return true;
}

bool ADroppedItem_Basic::CanInteract_Implementation(const FInteractionInfo Info) const
{
	if (IsPendingKillOrUnreachable()) { return false; }
	if (IsValid(Info.ControllerRef) == false) { return false; }
	if (IsValid(InteractingControllerRef)) {
		if (Info.ControllerRef == InteractingControllerRef) {
			return true;
		}
	}
	else {
		return true;
	}
	return false;
}

void ADroppedItem_Basic::BeginInteraction_Implementation(const FInteractionInfo Info)
{
	if (IsPendingKillOrUnreachable()) { return; }
	if (Info.ControllerRef && IsValid(Info.ControllerRef) == false) { return; }
	if (Info.PawnRef && IsValid(Info.PawnRef) == false) { return; }
	if (InteractingControllerRef && IsValid(InteractingControllerRef)) {
		if (InteractingControllerRef != Info.ControllerRef) {
			return;
		}
	}
	InteractingControllerRef = Info.ControllerRef;
	UDEPRECATED_InventoryComponent* InventoryRef = Info.PawnRef->FindComponentByClass<UDEPRECATED_InventoryComponent>();
	if (IsValid(InventoryRef)) {
		if (FItemSlot::IsValid(ItemData)) {
			FItemSlot Remainder = FItemSlot();
			InventoryRef->AddToInventory(ItemData, Remainder);
			ItemData = Remainder;
			if (!FItemSlot::IsValid(ItemData)) {
				Destroy();
			}
		}
	}
}

void ADroppedItem_Basic::EndInteraction_Implementation(const FInteractionInfo Info)
{
	if (IsPendingKillOrUnreachable()) { return; }
	if (Info.ControllerRef && IsValid(Info.ControllerRef) == false) { return; }
	if (InteractingControllerRef && IsValid(InteractingControllerRef)) {
		if (InteractingControllerRef != Info.ControllerRef) {
			return;
		}
	}
	InteractingControllerRef = nullptr;
}

void ADroppedItem_Basic::UpdateGraphics_Implementation()
{
	//Debug("UpdateGraphics_Implementation");
	UDEPRECATED_BasicItemSlotWidget* DisplayWidget;
	if (IsValid(ItemDataDisplayComponent1)) {
		//Debug("Display is Valid");
		if (IsValid(ItemDataDisplayComponent1->GetUserWidgetObject())) {
			//Debug("Display widget is Valid");
			DisplayWidget = Cast<UDEPRECATED_BasicItemSlotWidget>(ItemDataDisplayComponent1->GetUserWidgetObject());
			if (DisplayWidget) {
				//Debug("Cast Widget is Valid");
				DisplayWidget->SetItemData(ItemData);
				//DisplayWidget->UpdateGraphics();
			}
		}
	}
	if (ItemDataDisplayComponent2 && IsValid(ItemDataDisplayComponent2) && IsValid(ItemDataDisplayComponent2->GetUserWidgetObject())) {
		DisplayWidget = Cast<UDEPRECATED_BasicItemSlotWidget>(ItemDataDisplayComponent2->GetUserWidgetObject());
		if (DisplayWidget) {
			DisplayWidget->SetItemData(ItemData);
			//DisplayWidget->UpdateGraphics();
		}
	}
}


//*/