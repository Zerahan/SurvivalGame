// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SurvivalAICharacter.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Prediction.h"
#include "Characters/TeamAIController.h"
#include "Perception/AIPerceptionComponent.h"

// Sets default values
ASurvivalAICharacter::ASurvivalAICharacter()
	:Super()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SightRange		= 1000.f;
	SightRange_MAX	= 1250.f;
	SightAngle		= 45.f;
	SightMaxAge		= 60.f;
	HearingRange	= 1000.f;
	HearingMaxAge	= 60.f;
	AutoPossessAI				= EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass			= ATeamAIController::StaticClass();
	AIPerceptionComponentRef	= CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception"));
	if (AIPerceptionComponentRef) {
		AIPerceptionConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AIConfigSight"));
		AIPerceptionConfig_Sight->SightRadius								= SightRange;
		AIPerceptionConfig_Sight->LoseSightRadius							= SightRange_MAX;
		AIPerceptionConfig_Sight->PeripheralVisionAngleDegrees				= SightAngle;
		AIPerceptionConfig_Sight->AutoSuccessRangeFromLastSeenLocation		= 200.f;
		AIPerceptionConfig_Sight->SetMaxAge(SightMaxAge);
		AIPerceptionConfig_Sight->DetectionByAffiliation.bDetectEnemies		= true;
		AIPerceptionConfig_Sight->DetectionByAffiliation.bDetectNeutrals	= true;
		AIPerceptionConfig_Sight->DetectionByAffiliation.bDetectFriendlies	= true;
		
		AIPerceptionComponentRef->ConfigureSense(*AIPerceptionConfig_Sight);
		AIPerceptionComponentRef->SetDominantSense(AIPerceptionConfig_Sight->Implementation);

		AIPerceptionConfig_Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("AIConfigHearing"));
		AIPerceptionConfig_Hearing->HearingRange								= HearingRange;
		AIPerceptionConfig_Hearing->SetMaxAge(HearingMaxAge);
		AIPerceptionConfig_Hearing->DetectionByAffiliation.bDetectEnemies		= true;
		AIPerceptionConfig_Hearing->DetectionByAffiliation.bDetectNeutrals		= true;
		AIPerceptionConfig_Hearing->DetectionByAffiliation.bDetectFriendlies	= true;
		AIPerceptionComponentRef->ConfigureSense(*AIPerceptionConfig_Hearing);
	}
	CurrentBehaviorState	= EAIBehaviorState::BS_IDLE;
	LastBehaviorState		= EAIBehaviorState::BS_IDLE;
}

void ASurvivalAICharacter::PossessedBy(AController * NewTeamAIControllerRef)
{
	Super::PossessedBy(NewTeamAIControllerRef);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Test")));
	UE_LOG(LogTemp, Warning, TEXT("Test"));
	if (AIPerceptionComponentRef && IsValid(AIPerceptionComponentRef)) {
		AIPerceptionComponentRef->OnPerceptionUpdated.RemoveDynamic(this, &ASurvivalAICharacter::OnPerceptionUpdated);
		AIPerceptionComponentRef->OnTargetPerceptionUpdated.RemoveDynamic(this, &ASurvivalAICharacter::OnTargetPerceptionUpdated);
		AIPerceptionComponentRef->OnTargetPerceptionInfoUpdated.RemoveDynamic(this, &ASurvivalAICharacter::OnTargetPerceptionInfoUpdated);
	}
	TeamAIControllerRef = Cast<ATeamAIController>(NewTeamAIControllerRef);
	//TeamAIControllerRef = nullptr;
	//ATeamAIController* conRef = Cast<ATeamAIController>(AIController_);
	if (TeamAIControllerRef && IsValid(TeamAIControllerRef)) {
		//TeamAIControllerRef = conRef;
		TeamAIControllerRef->SetTeamID(TeamID);
		TeamAIControllerRef->SetPerceptionComponent(*AIPerceptionComponentRef);
		//AIPerceptionComponentRef = TeamAIControllerRef->GetAIPerceptionComponent();
		if (AIPerceptionComponentRef && IsValid(AIPerceptionComponentRef)) {
			AIPerceptionComponentRef->OnPerceptionUpdated.AddDynamic(this, &ASurvivalAICharacter::OnPerceptionUpdated);
			AIPerceptionComponentRef->OnTargetPerceptionUpdated.AddDynamic(this, &ASurvivalAICharacter::OnTargetPerceptionUpdated);
			AIPerceptionComponentRef->OnTargetPerceptionInfoUpdated.AddDynamic(this, &ASurvivalAICharacter::OnTargetPerceptionInfoUpdated);
		}
	}
}

void ASurvivalAICharacter::SetSightRange(const float NewSightRange, const float NewSightRange_MAX)
{
	SightRange		= NewSightRange;
	SightRange_MAX	= NewSightRange_MAX;
	if (AIPerceptionConfig_Sight) {
		AIPerceptionConfig_Sight->SightRadius = SightRange;
		AIPerceptionConfig_Sight->LoseSightRadius = SightRange_MAX;
	}
}

void ASurvivalAICharacter::SetSightAngle(const float NewSightAngle)
{
	SightAngle	= NewSightAngle;
	if (AIPerceptionConfig_Sight) {
		AIPerceptionConfig_Sight->PeripheralVisionAngleDegrees = SightAngle;
	}
}

void ASurvivalAICharacter::OnTargetPerceptionInfoUpdated_Implementation(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Updated...")));
	UE_LOG(LogTemp, Warning, TEXT("Updated..."));
	AActor* StimTarget = nullptr;
	if (UpdateInfo.Target.IsValid()) {
		StimTarget = UpdateInfo.Target.Get();
	}

	if (!IsValid(StimTarget)) { return; }

	if (UpdateInfo.Stimulus.IsActive()) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Detected: %s"), *StimTarget->GetName()));
		UE_LOG(LogTemp, Warning, TEXT("Detected: %s"), *StimTarget->GetName());
	}

	if (UpdateInfo.Stimulus.IsExpired() || !UpdateInfo.Stimulus.IsValid()) {
		FActorPerceptionInfo Info = *AIPerceptionComponentRef->GetActorInfo(*StimTarget);
		if (!Info.HasAnyCurrentStimulus()) {
			PerceptionPriorityList.Remove(StimTarget);
			return;
		}
	}

	if (UpdateInfo.Stimulus.Type == UAISense::GetSenseID(UAISense_Sight::StaticClass())) {
		if (UpdateInfo.Stimulus.IsActive() && TeamAIControllerRef->GetTeamAttitudeTowards(*StimTarget) == ETeamAttitude::Hostile) {
			PerceptionPriorityList.AddUnique(StimTarget);
			FVector ActorLocation = GetActorLocation();
			UAIPerceptionComponent* Perception = AIPerceptionComponentRef;

			//https://answers.unrealengine.com/questions/606320/help-with-c-sort-by-predicate.html
			// Maybe should switch to Algo::Sort if there are errors. Probably should also move the def to a struct for reusability

			PerceptionPriorityList.Sort([ActorLocation, Perception](const AActor& A, const AActor& B) {
				FActorPerceptionInfo InfoA = *Perception->GetActorInfo(A);
				FActorPerceptionInfo InfoB = *Perception->GetActorInfo(B);
				// Having a current stimulus takes priority over not having one.
				// If both are the same, sort by distance
				if (InfoA.HasAnyCurrentStimulus() == InfoB.HasAnyCurrentStimulus()) {
				//	if (InfoA.LastSensedStimuli.IsValidIndex(0) && InfoB.LastSensedStimuli.IsValidIndex(0)) {
				//		return InfoA.LastSensedStimuli[0].GetAge() > InfoB.LastSensedStimuli[0].GetAge();
				//	}
				//	else {
						return FVector::DistSquared(ActorLocation, A.GetActorLocation()) > FVector::DistSquared(ActorLocation, B.GetActorLocation());
				//	}
				}
				else {
					return InfoB.HasAnyCurrentStimulus();
				}
			});
		}
	}
}

void ASurvivalAICharacter::SetBehaviorState(const EAIBehaviorState NewBehaviorState)
{
	LastBehaviorState		= CurrentBehaviorState;
	CurrentBehaviorState	= NewBehaviorState;
	OnBehaviorStateChanged();
}

TArray<AActor*> ASurvivalAICharacter::GetPerceptionPriorityList() const
{
	return PerceptionPriorityList;
}

void ASurvivalAICharacter::ExpireLatestForTopPerception()
{
	FActorPerceptionInfo Info = *AIPerceptionComponentRef->GetActorInfo(*LastTopPerceptionRef);
	Info.LastSensedStimuli[LastSensedStimIndex].MarkExpired();
}

AActor* ASurvivalAICharacter::GetPerceptionTopPriority(FVector& LastLocation, bool& IsLocationValid)
{
	IsLocationValid = false;
	if (PerceptionPriorityList.Num() > 0) {
		for (int i = 0; i < PerceptionPriorityList.Num(); i++) {
			FActorPerceptionInfo Info = *AIPerceptionComponentRef->GetActorInfo(*PerceptionPriorityList[i]);
			for (int s = Info.LastSensedStimuli.Num()-1; s >= 0; s--) {
				if (!Info.LastSensedStimuli[s].IsExpired()) {
					LastSensedStimIndex = s;
					LastLocation = Info.LastSensedStimuli[s].StimulusLocation;
					IsLocationValid = true;
					LastTopPerceptionRef = PerceptionPriorityList[i];
					if (Info.LastSensedStimuli[s].IsActive()) {
						return PerceptionPriorityList[i];
					}
					else {
						//return nullptr;
					}
				}
			}
		}
		return PerceptionPriorityList[0];
	}
	LastTopPerceptionRef = nullptr;
	return nullptr;
}

void ASurvivalAICharacter::GetActorEyesViewPoint(FVector & OutLocation, FRotator & OutRotation) const
{
	OutLocation = GetActorEyesLocation();
	OutRotation = GetActorEyesRotation();
}

FVector ASurvivalAICharacter::GetActorEyesLocation_Implementation() const
{
	return GetPawnViewLocation();
}

FRotator ASurvivalAICharacter::GetActorEyesRotation_Implementation() const
{
	return GetViewRotation();
}