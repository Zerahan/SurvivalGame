// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SurvivalGame.h"
#include "UObject/Interface.h"
#include "StatusInterface.generated.h"

UENUM(BlueprintType)
enum class EStatusType : uint8
{
	Health		UMETA(DisplayName = "Health"),
	Stamina		UMETA(DisplayName = "Stamina"),
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UStatusInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SURVIVALGAME_API IStatusInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Status")
	bool CanModifyStatus(const EStatusType StatusType) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Status")
	float GetStatus(const EStatusType StatusType) const;

	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Status")
	//void SetStatus(const EStatusType StatusType, const float Amount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Status")
	void ModifyStatus(const EStatusType StatusType, const float Amount);
};
