// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FakeComputerApplicationWidget_Basic.h"
#include "Props/Computers/FakeComputerApplication_Basic.h"

UFakeComputerApplicationWidget_Basic::UFakeComputerApplicationWidget_Basic(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

inline UFakeComputerApplication_Basic* UFakeComputerApplicationWidget_Basic::GetAppRef() const { return ComputerAppRef; }

void UFakeComputerApplicationWidget_Basic::SetAppRef_Implementation(UFakeComputerApplication_Basic* NewComputerAppRef)
{
	ComputerAppRef = NewComputerAppRef;
}
