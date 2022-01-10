// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MenuInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMenuInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORYSYSTEM_API IMenuInterface
{
	GENERATED_BODY()

public:
	/**
	* Runs UpdateGraphics on this widget and sub widgets. For cosmetic changes only.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Menu")
	void UpdateGraphics();

	/**
	* For wiping and rebuilding any constructed subwidgets or objects.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Menu")
	void Rebuild();
	
	/**
	* Usually for making widgets visible and setting inputmode to game and ui
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Menu")
	void OpenMenu();

	/**
	* Usually for collapsing widgets and setting inputmode to game only
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Menu")
	void CloseMenu();

	/**
	* Toggles between OpenMenu and CloseMenu
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Menu")
	void ToggleMenu();
};
