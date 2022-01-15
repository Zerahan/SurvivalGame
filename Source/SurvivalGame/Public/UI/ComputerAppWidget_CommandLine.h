// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/ComputerAppWidget_Basic.h"
#include "ComputerAppWidget_CommandLine.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom))
class SURVIVALGAME_API UComputerAppWidget_CommandLine : public UComputerAppWidget_Basic
{
	GENERATED_BODY()

public:
	UComputerAppWidget_CommandLine(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
	
};
