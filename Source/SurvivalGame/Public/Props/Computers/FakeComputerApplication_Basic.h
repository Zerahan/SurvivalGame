// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SurvivalGame.h"
#include "UObject/NoExportTypes.h"
#include "Props/Computers/ComputerTypes.h"
#include "FakeComputerApplication_Basic.generated.h"

class UFakeComputerApplicationWidget_Basic;
class AAccessibleComputer_Basic;
class UUserWidget;

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom))
class SURVIVALGAME_API UFakeComputerApplication_Basic : public UObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (BlueprintProtected))
	FName AppName;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(BlueprintProtected))
	TSubclassOf<UFakeComputerApplicationWidget_Basic> WidgetClass;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(BlueprintProtected))
	uint8 RequiredGroupID;

public:
	UFakeComputerApplication_Basic();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default|Computer")
	void Initialize();
	virtual void Initialize_Implementation();

	UFUNCTION(BlueprintCallable, Category = "Default|Computer")
	uint8 GetRequiredGroupID() const;

	UFUNCTION(BlueprintCallable, Category = "Default|Computer")
	FName GetDisplayName() const;

	UFUNCTION(BlueprintCallable, Category = "Default|Computer")
	AAccessibleComputer_Basic* GetOwningComputer() const;

	UFUNCTION(BlueprintCallable, Category = "Default|Computer")
	void AssignGroupIDFromPrivilages(const EFakeUserPrivilages Level, const EFakeUserPrivilages Group);

	UFUNCTION(BlueprintCallable, Category = "Default|Computer")
	void AssignGroupID(const uint8 GroupID);

	UFUNCTION(BlueprintCallable, Category = "Default|Computer")
	TSubclassOf<UFakeComputerApplicationWidget_Basic> GetWidgetClass() const;

	UFUNCTION(BlueprintCallable, Category = "Default|Computer|UI")
	UUserWidget* GenerateWidget(APlayerController* Controller);

	UFUNCTION(BlueprintCallable, Category = "Default|Computer")
	UFakeComputerApplication_Basic* CopyApp();
};
