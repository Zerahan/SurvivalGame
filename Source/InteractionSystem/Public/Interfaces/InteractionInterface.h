// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

USTRUCT(BlueprintType)
struct INTERACTIONSYSTEM_API FInteractionInfo {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
	AController* ControllerRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
	APawn* PawnRef;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
	UPrimitiveComponent* HitComponent;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
	//AActor* TargetActorRef;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INTERACTIONSYSTEM_API IInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	* Checks if the target object can currently be interacted with.
	* @param Info The info to provide the function call for varification purposes.
	* @return Always returns false unless overriden.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	bool CanInteract(const FInteractionInfo Info) const;

	/**
	* Called when an interaction begins. Pushing a button down calls this.
	* @param Info The info to provide the function call for varification purposes.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void BeginInteraction(const FInteractionInfo Info);

	/**
	* Called when an interaction ends. Letting go of a pushed button calls this.
	* @param Info The info to provide the function call for varification purposes.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void EndInteraction(const FInteractionInfo Info);

	/**
	* Manually-called version of EndInteraction.
	* @param Info The info to provide the function call for varification purposes.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void CancelInteraction(const FInteractionInfo Info);
};
