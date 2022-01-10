// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SurvivalGame.h"
#include "StatusTypes.generated.h"


UENUM(BlueprintType)
enum class EHealthState : uint8
{
	IsAlive		UMETA(DisplayName = "Alive"),
	IsDead		UMETA(DisplayName = "Dead"),
	IsRessing	UMETA(DisplayName = "Ressing"),
	IsDying		UMETA(DisplayName = "Dying"),
};