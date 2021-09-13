// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/TeamAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Utility/UtilityFunctionLibrary.h"

ATeamAIController::ATeamAIController()
{
	SetGenericTeamId(FGenericTeamId((uint8)EGameTeam::T_Neutral));
}

ETeamAttitude::Type ATeamAIController::GetTeamAttitudeTowards(const AActor & Other) const
{
	if (GetGenericTeamId().GetId() == (uint8)EGameTeam::T_Monster) {
		return ETeamAttitude::Hostile;
	}
	if (GetGenericTeamId().GetId() != (uint8)EGameTeam::T_Neutral) {
		if (const APawn* OtherPawn = Cast<APawn>(&Other)) {
			if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController())) {
				FGenericTeamId OtherTeamID = TeamAgent->GetGenericTeamId();
				if (OtherTeamID.GetId() == (uint8)EGameTeam::T_Monster) {
					return ETeamAttitude::Hostile;
				}
				if (OtherTeamID.GetId() != (uint8)EGameTeam::T_Neutral) {
					if (OtherTeamID == GetGenericTeamId()) {
						return ETeamAttitude::Friendly;
					}
					else {
						return ETeamAttitude::Hostile;
					}
				}
			}
		}
	}
	return ETeamAttitude::Neutral;
}

void ATeamAIController::SetTeamID(EGameTeam NewTeamID)
{
	SetGenericTeamId(FGenericTeamId((uint8)NewTeamID));
}

ETeamAttitude::Type ATeamAIController::GetTeamAttitudeTowards_BP(const AActor * Other) const
{
	return GetTeamAttitudeTowards(*Other);
}
