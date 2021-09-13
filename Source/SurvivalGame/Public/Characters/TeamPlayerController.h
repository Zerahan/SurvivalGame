// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "Characters/GameTeams.h"
#include "TeamPlayerController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom))
class SURVIVALGAME_API ATeamPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:
	ATeamPlayerController();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Team")
	EGameTeam TeamID = EGameTeam::T_Neutral;

	UFUNCTION(BlueprintCallable, Category = "Team")
	void SetTeamID(EGameTeam NewTeamID);

private:
	UFUNCTION()
	FGenericTeamId GetGenericTeamId() const;
};
