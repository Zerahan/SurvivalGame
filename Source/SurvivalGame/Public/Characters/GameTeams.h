// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SurvivalGame.h"
#include "GenericTeamAgentInterface.h"
#include "GameTeams.generated.h"

UENUM(BlueprintType)
enum class EGameTeam : uint8
{
	T_Neutral	UMETA(DisplayName = "Neutral"),
	T_Monster	UMETA(DisplayName = "Monster"),
	T_Team1		UMETA(DisplayName = "Team 1"),
	T_Team2		UMETA(DisplayName = "Team 2"),
	T_Team3		UMETA(DisplayName = "Team 3"),
	T_Team4		UMETA(DisplayName = "Team 4"),
};

USTRUCT(BlueprintType)
struct SURVIVALGAME_API FTeamAttitude {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TEnumAsByte<ETeamAttitude::Type>> Attitude;
};