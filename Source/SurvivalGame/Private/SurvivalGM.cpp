// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalGM.h"
#include "Characters/TeamPlayerController.h"
#include "Characters/SurvivalCharacter.h"

ASurvivalGM::ASurvivalGM(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PlayerControllerClass = ATeamPlayerController::StaticClass();
	DefaultPawnClass = ASurvivalCharacter::StaticClass();
}