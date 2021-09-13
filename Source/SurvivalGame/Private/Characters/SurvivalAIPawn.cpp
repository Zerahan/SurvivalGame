// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SurvivalAIPawn.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Characters/TeamAIController.h"
#include "Perception/AIPerceptionComponent.h"

// Sets default values
ASurvivalAIPawn::ASurvivalAIPawn()
	:Super()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseEyeHeight = 0;
	SightRange = 1000.f;
	SightRange_MAX = 1250.f;
	SightAngle = 45.f;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ATeamAIController::StaticClass();
	AIPerceptionComponentRef = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception"));
	AIPerceptionConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AIConfigSight"));
	if (AIPerceptionComponentRef) {
		AIPerceptionConfig_Sight->SightRadius								= SightRange;
		AIPerceptionConfig_Sight->LoseSightRadius							= SightRange_MAX;
		AIPerceptionConfig_Sight->PeripheralVisionAngleDegrees				= SightAngle;
		AIPerceptionConfig_Sight->DetectionByAffiliation.bDetectEnemies		= true;
		AIPerceptionConfig_Sight->DetectionByAffiliation.bDetectNeutrals	= true;
		AIPerceptionConfig_Sight->DetectionByAffiliation.bDetectFriendlies	= true;
		
		AIPerceptionComponentRef->ConfigureSense(*AIPerceptionConfig_Sight);
		AIPerceptionComponentRef->SetDominantSense(AIPerceptionConfig_Sight->Implementation);
	}
}

// Called when the game starts or when spawned
void ASurvivalAIPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASurvivalAIPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASurvivalAIPawn::PossessedBy(AController * NewTeamAIControllerRef)
{
	Super::PossessedBy(NewTeamAIControllerRef);

	if (IsValid(AIPerceptionComponentRef)) {
		AIPerceptionComponentRef->OnPerceptionUpdated.RemoveDynamic(this, &ASurvivalAIPawn::OnPerceptionUpdated);
		AIPerceptionComponentRef->OnTargetPerceptionUpdated.RemoveDynamic(this, &ASurvivalAIPawn::OnTargetPerceptionUpdated);
		AIPerceptionComponentRef->OnTargetPerceptionInfoUpdated.RemoveDynamic(this, &ASurvivalAIPawn::OnTargetPerceptionInfoUpdated);
	}
	TeamAIControllerRef = Cast<ATeamAIController>(NewTeamAIControllerRef);
	//TeamAIControllerRef = nullptr;
	//ATeamAIController* conRef = Cast<ATeamAIController>(AIController_);
	if (NewTeamAIControllerRef && IsValid(NewTeamAIControllerRef)) {
		//TeamAIControllerRef = conRef;
		TeamAIControllerRef->SetTeamID(TeamID);
		TeamAIControllerRef->SetPerceptionComponent(*AIPerceptionComponentRef);
		//AIPerceptionComponentRef = TeamAIControllerRef->GetAIPerceptionComponent();
		if (AIPerceptionComponentRef && IsValid(AIPerceptionComponentRef)) {
			AIPerceptionComponentRef->OnPerceptionUpdated.AddDynamic(this, &ASurvivalAIPawn::OnPerceptionUpdated);
			AIPerceptionComponentRef->OnTargetPerceptionUpdated.AddDynamic(this, &ASurvivalAIPawn::OnTargetPerceptionUpdated);
			AIPerceptionComponentRef->OnTargetPerceptionInfoUpdated.AddDynamic(this, &ASurvivalAIPawn::OnTargetPerceptionInfoUpdated);
		}
	}
}

void ASurvivalAIPawn::SetSightRange(float SightRange_, float SightRange_MAX_)
{
	SightRange = SightRange_;
	SightRange_MAX = SightRange_MAX_;
	if (AIPerceptionConfig_Sight) {
		AIPerceptionConfig_Sight->SightRadius = SightRange;
		AIPerceptionConfig_Sight->LoseSightRadius = SightRange_MAX;
	}
}

void ASurvivalAIPawn::SetSightAngle(float SightAngle_)
{
	SightAngle = SightAngle_;
	if (AIPerceptionConfig_Sight) {
		AIPerceptionConfig_Sight->PeripheralVisionAngleDegrees = SightAngle;
	}
}

TArray<AActor*> ASurvivalAIPawn::GetPerceptionPriorityList() const
{
	return PerceptionPriorityList;
}

AActor* ASurvivalAIPawn::GetPerceptionTopPriority() const
{
	if (PerceptionPriorityList.Num() > 0) {
		return PerceptionPriorityList[0];
	}
	return nullptr;
}

void ASurvivalAIPawn::GetActorEyesViewPoint(FVector & OutLocation, FRotator & OutRotation) const
{
	OutLocation = GetActorEyesLocation();
	OutRotation = GetActorEyesRotation();
}

FVector ASurvivalAIPawn::GetActorEyesLocation_Implementation() const
{
	return GetPawnViewLocation();
}

FRotator ASurvivalAIPawn::GetActorEyesRotation_Implementation() const
{
	return GetViewRotation();
}

