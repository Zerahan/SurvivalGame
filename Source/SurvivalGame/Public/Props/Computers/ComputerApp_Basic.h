// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SurvivalGame.h"
#include "UObject/NoExportTypes.h"
#include "Props/Computers/ComputerTypes.h"
#include "ComputerApp_Basic.generated.h"

class UComputerAppWidget_Basic;
class UUserWidget;

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom))
class SURVIVALGAME_API UComputerApp_Basic : public UObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (BlueprintProtected))
	FName AppName;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(BlueprintProtected))
	TSubclassOf<UComputerAppWidget_Basic> WidgetClass;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(BlueprintProtected))
	uint8 RequiredGroupID;

public:
	UComputerApp_Basic();

	UFUNCTION(BlueprintCallable, Category = "Default|Computer")
	uint8 GetRequiredGroupID() const;

	UFUNCTION(BlueprintCallable, Category = "Default|Computer")
	FName GetDisplayName() const;

	UFUNCTION(BlueprintCallable, Category = "Default|Computer")
	void AssignGroupID(const EFakeUserPrivilages Level, const EFakeUserPrivilages Group);

	UFUNCTION(BlueprintCallable, Category = "Default|Computer")
	TSubclassOf<UComputerAppWidget_Basic> GetWidgetClass() const;

	UFUNCTION(BlueprintCallable, Category = "Default|Computer|UI")
	UUserWidget* GenerateWidget(APlayerController* Controller);

	UFUNCTION(BlueprintCallable, Category = "Default|Computer")
	UComputerApp_Basic* CopyApp();
};
