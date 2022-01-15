// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ComputerAppWidget_Basic.generated.h"

class UComputerApp_Basic;

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom))
class SURVIVALGAME_API UComputerAppWidget_Basic : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadOnly, Category = "Default|ComputerApp", meta=(AllowPrivateAccess="true", ExposeOnSpawn = "true"))
	UComputerApp_Basic* ComputerAppRef;

public:
	UComputerAppWidget_Basic(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintCallable, Category = "Default|ComputerApp")
	UComputerApp_Basic* GetAppRef() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Default|ComputerApp")
	void SetAppRef(UComputerApp_Basic* NewComputerAppRef);
	virtual void SetAppRef_Implementation(UComputerApp_Basic* NewComputerAppRef);
	
};
