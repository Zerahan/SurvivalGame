// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/DroppedItem_Generic.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Inventory/UI/WidgetInventorySlot_Basic.h"
#include "Inventory/InventorySlotData_Stack.h"

// Sets default values
ADroppedItem_Generic::ADroppedItem_Generic()
	: Super()
{
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
	
	ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/GameCore/Dev/UI/BasicInventorySlot"));
	if (WidgetClass.Succeeded()) {
		ItemDataDisplayComponent1->SetWidgetClass(WidgetClass.Class);
		ItemDataDisplayComponent2->SetWidgetClass(WidgetClass.Class);
	}
}

// Called when the game starts or when spawned
void ADroppedItem_Generic::BeginPlay()
{
	if (ItemDataBuilder.IsValid()) {
		FItemStaticData_Stack LoadedStaticData = *ItemDataBuilder.StaticDataHandle.GetRow<FItemStaticData_Stack>("Getting ItemStaticData for stack builder");
		if ( FItemStaticData_Stack::IsValid(LoadedStaticData)) {
			UInventorySlotData_Stack* NewSlot = NewObject<UInventorySlotData_Stack>(this, (LoadedStaticData.BasicData.InstancedDataClass != nullptr ? LoadedStaticData.BasicData.InstancedDataClass : UInventorySlotData_Stack::StaticClass()));
			if (NewSlot) {
				NewSlot->SetFromData(ItemDataBuilder);
				SetItemSlot(NewSlot);
			}
		}
	}

	Super::BeginPlay();
}

void ADroppedItem_Generic::UpdateGraphics_Implementation()
{
	UWidgetInventorySlot_Basic* DisplayWidget;
	if (IsValid(ItemDataDisplayComponent1) && IsValid(ItemDataDisplayComponent1->GetUserWidgetObject())) {
		DisplayWidget = Cast<UWidgetInventorySlot_Basic>(ItemDataDisplayComponent1->GetUserWidgetObject());
		if (DisplayWidget) {
			DisplayWidget->SetDataFromSlot(ItemData);
		}
	}
	if (IsValid(ItemDataDisplayComponent2) && IsValid(ItemDataDisplayComponent2->GetUserWidgetObject())) {
		DisplayWidget = Cast<UWidgetInventorySlot_Basic>(ItemDataDisplayComponent2->GetUserWidgetObject());
		if (DisplayWidget) {
			DisplayWidget->SetDataFromSlot(ItemData);
		}
	}
	Super::UpdateGraphics_Implementation();
}
