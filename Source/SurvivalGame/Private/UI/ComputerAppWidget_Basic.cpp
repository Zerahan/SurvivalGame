// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ComputerAppWidget_Basic.h"
#include "Props/Computers/ComputerApp_Basic.h"

UComputerAppWidget_Basic::UComputerAppWidget_Basic(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UComputerAppWidget_Basic::NativeConstruct()
{
	Super::NativeConstruct();
}

void UComputerAppWidget_Basic::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

inline UComputerApp_Basic* UComputerAppWidget_Basic::GetAppRef() const { return ComputerAppRef; }

void UComputerAppWidget_Basic::SetAppRef_Implementation(UComputerApp_Basic* NewComputerAppRef)
{
	ComputerAppRef = NewComputerAppRef;
}
