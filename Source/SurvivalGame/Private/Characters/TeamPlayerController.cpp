// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/TeamPlayerController.h"

ATeamPlayerController::ATeamPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	TeamID = EGameTeam::T_Neutral;
}

FGenericTeamId ATeamPlayerController::GetGenericTeamId() const
{
	return FGenericTeamId((uint8)TeamID);
}

void ATeamPlayerController::SetTeamID(EGameTeam NewTeamID)
{
	TeamID = NewTeamID;
	SetGenericTeamId(FGenericTeamId((uint8)TeamID));
}
