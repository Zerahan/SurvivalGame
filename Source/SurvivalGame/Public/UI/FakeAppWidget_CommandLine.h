// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/FakeComputerApplicationWidget_Basic.h"
#include "FakeAppWidget_CommandLine.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom))
class SURVIVALGAME_API UFakeAppWidget_CommandLine : public UFakeComputerApplicationWidget_Basic
{
	GENERATED_BODY()

public:
	UFakeAppWidget_CommandLine(const FObjectInitializer& ObjectInitializer);

	//virtual void NativeConstruct() override;
	//virtual void NativeOnInitialized() override;
	
};
