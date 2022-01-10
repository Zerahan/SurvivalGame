// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/DroppedItem_Tool.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Inventory/InventorySlotData_Tool.h"
#include "Inventory/ItemTool_Basic.h"

// Sets default values
ADroppedItem_Tool::ADroppedItem_Tool()
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
}

// Called when the game starts or when spawned
void ADroppedItem_Tool::BeginPlay()
{
	if (ItemDataBuilder.IsValid()) {
		FItemStaticData_Tool LoadedStaticData = *ItemDataBuilder.StaticDataHandle.GetRow<FItemStaticData_Tool>("Getting ItemStaticData for tool builder...");
		if (FItemStaticData_Tool::IsValid(LoadedStaticData)) {
			UInventorySlotData_Tool* NewSlot = NewObject<UInventorySlotData_Tool>(this, (LoadedStaticData.BasicData.InstancedDataClass != nullptr ? LoadedStaticData.BasicData.InstancedDataClass : UInventorySlotData_Tool::StaticClass()));
			if (NewSlot) {
				NewSlot->SetFromData(ItemDataBuilder);
				SetItemSlot(NewSlot);
			}
		}
	}

	Super::BeginPlay();
}

void ADroppedItem_Tool::UpdateGraphics_Implementation()
{
	if (IsValid(RootStaticMeshComponent)) {
		FItemStaticData_Tool StaticData;// = UInventorySlotData_Tool::LookupStaticData(ItemData);
		if (FItemStaticData_Tool::IsValid(StaticData)){
			auto* Component = StaticData.EquippedClass.GetDefaultObject()->GetRootComponent();
			if (auto* CastComponent = Cast<UStaticMeshComponent>(Component)) {
				RootStaticMeshComponent->SetStaticMesh(CastComponent->GetStaticMesh());
			}
		}
	}
	Super::UpdateGraphics_Implementation();
}
