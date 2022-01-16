// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FakeComputerApplicationWidget_Basic.generated.h"

class UFakeComputerApplication_Basic;

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom))
class SURVIVALGAME_API UFakeComputerApplicationWidget_Basic : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadOnly, Category = "Default|ComputerApp", meta=(AllowPrivateAccess="true", ExposeOnSpawn = "true"))
	UFakeComputerApplication_Basic* ComputerAppRef;

public:
	UFakeComputerApplicationWidget_Basic(const FObjectInitializer& ObjectInitializer);

	//virtual void NativeConstruct() override;
	//virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintCallable, Category = "Default|ComputerApp")
	UFakeComputerApplication_Basic* GetAppRef() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default|ComputerApp")
	void SetAppRef(UFakeComputerApplication_Basic* NewComputerAppRef);
	virtual void SetAppRef_Implementation(UFakeComputerApplication_Basic* NewComputerAppRef);
	
};
