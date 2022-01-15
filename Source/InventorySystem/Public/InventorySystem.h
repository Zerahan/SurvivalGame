// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

/*
#ifndef Debug
#define Debug(str, ...) \
{ \
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT(str), __VA_ARGS__)); \
	UE_LOG(LogTemp, Warning, TEXT(str), __VA_ARGS__); \
}
#endif
//*/

/**
 * 
 */
class FInventorySystemModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
