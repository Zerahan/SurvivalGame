// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SurvivalGame.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "Characters/GameTeams.h"
#include "TeamAIController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom))
class SURVIVALGAME_API ATeamAIController : public AAIController
{
	GENERATED_BODY()

	ATeamAIController();
public:
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UFUNCTION(BlueprintCallable, Category = "Team")
	void SetTeamID(EGameTeam NewTeamID);

	UFUNCTION(BlueprintCallable, Category = "Team")
	ETeamAttitude::Type GetTeamAttitudeTowards_BP(const AActor* Other) const;
};
