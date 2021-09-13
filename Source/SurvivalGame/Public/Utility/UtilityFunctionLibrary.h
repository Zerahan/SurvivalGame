// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SlateWrapperTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UtilityFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UUtilityFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Event Reply")
	static bool IsEventReplyHandled(FEventReply Reply);

	UFUNCTION()
	static void PrintDebug(const FString& str, const FColor Color = FColor::Red);

	UFUNCTION(BlueprintCallable, Category = "String")
	static int CompareString(const FString& StringA, const FString& StringB, const bool IsDescendingOrder = true);
};
