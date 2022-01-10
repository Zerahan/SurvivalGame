// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Item_Basic.h"

// Sets default values
AItem_Basic::AItem_Basic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItem_Basic::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem_Basic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

