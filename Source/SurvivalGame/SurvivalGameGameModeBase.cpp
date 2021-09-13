// Copyright Epic Games, Inc. All Rights Reserved.


#include "SurvivalGameGameModeBase.h"
#include "Characters/TeamPlayerController.h"
#include "Characters/SurvivalCharacter.h"

ASurvivalGameGameModeBase::ASurvivalGameGameModeBase(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerControllerClass = ATeamPlayerController::StaticClass();
	DefaultPawnClass = ASurvivalCharacter::StaticClass();
}
