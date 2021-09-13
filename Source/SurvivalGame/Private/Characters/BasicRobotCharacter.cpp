// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/BasicRobotCharacter.h"
#include "Components/SpotLightComponent.h"
#include "Components/PointLightComponent.h"
#include "Characters/TeamAIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "Utility/UtilityFunctionLibrary.h"

ABasicRobotCharacter::ABasicRobotCharacter()
	:Super()
{
	LookRotationYawLimit = FVector2D(-80,80);
	LookRotationPitchLimit = FVector2D(-45, 45);

	IsLookRotating = false;
	UseLookRotation = false;
	/*
	const ConstructorHelpers::FObjectFinder<USkeletalMesh> meshObject_CON(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin"));
	if (meshObject_CON.Succeeded()) {
		GetMesh()->SetSkeletalMesh(meshObject_CON.Object);
	}
	const ConstructorHelpers::FObjectFinder<UAnimBlueprint> animationObject_CON(TEXT("/Game/Mannequin/Animations/ThirdPerson_AnimBP"));
	if (animationObject_CON.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(animationObject_CON.Object->GeneratedClass);
	}//*/
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));

	CameraMount = CreateDefaultSubobject<USceneComponent>(TEXT("CameraMount"));
	CameraMount->SetupAttachment(GetMesh(), FName("head"));
	CameraMount->SetRelativeLocationAndRotation(FVector(5, 0, 0), FRotator(0, 90, -90));

	CameraLookOverride = CreateDefaultSubobject<USceneComponent>(TEXT("CameraLookOverride"));
	CameraLookOverride->SetupAttachment(CameraMount);

	CameraMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CameraMesh"));
	CameraMesh->SetupAttachment(CameraLookOverride);
	CameraMesh->SetRelativeLocation(FVector(45, 0, 5));
	CameraMesh->SetRelativeScale3D(FVector(0.75f, 1.f, 1.f));

	/*
	const ConstructorHelpers::FObjectFinder<UStaticMesh> cameraMeshObject_CON(TEXT("/Engine/EditorMeshes/MatineeCam_SM"));
	if (cameraMeshObject_CON.Succeeded()) {
		CameraMesh->SetStaticMesh(cameraMeshObject_CON.Object);
	}//*/


	CameraLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("VisionLight"));
	CameraLight->SetupAttachment(CameraMesh);
	
	CameraAccentLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("CameraAccentLight"));
	CameraAccentLight->SetupAttachment(CameraLight);
	CameraAccentLight->SetAttenuationRadius(50);

	/*
	const ConstructorHelpers::FObjectFinder<UBlackboardData> blackboardObject_CON(TEXT("/Game/SurvivalGame/Dev/Blueprints/AI/BasicRobot_BB"));
	if (blackboardObject_CON.Succeeded()) {
		BlackboardDataRef = blackboardObject_CON.Object;
	}
	const ConstructorHelpers::FObjectFinder<UBehaviorTree> behaviorTreeObject_CON(TEXT("/Game/SurvivalGame/Dev/Blueprints/AI/BasicRobot_BT"));
	if (behaviorTreeObject_CON.Succeeded()) {
		BehaviorTreeRef = behaviorTreeObject_CON.Object;
	}//*/
}

// Called when the game starts or when spawned
void ABasicRobotCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABasicRobotCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsLookRotating) {
		if (UseLookRotation) {
			if (LookRotation.Equals(CameraLookOverride->GetComponentRotation(), 0.1f)) {
				IsLookRotating = false;
				CameraLookOverride->SetWorldRotation(LookRotation);
			}
			else {
				CameraLookOverride->SetWorldRotation(FMath::RInterpTo(CameraLookOverride->GetComponentRotation(), LookRotation, DeltaTime, 5.f));
			}
		}
		else {
			if (GetActorRotation().Equals(CameraLookOverride->GetComponentRotation(), 0.1f)) {
				IsLookRotating = false;
				CameraLookOverride->SetWorldRotation(GetActorRotation());
			}
			else {
				CameraLookOverride->SetWorldRotation(FMath::RInterpTo(CameraLookOverride->GetComponentRotation(), GetActorRotation(), DeltaTime, 5.f));
			}
		}
	}
}

void ABasicRobotCharacter::PossessedBy(AController* NewTeamAIControllerRef)
{
	Super::PossessedBy(NewTeamAIControllerRef);
	if (!IsValid(TeamAIControllerRef)) { return; }
	if (!BlackboardDataRef) { return; }
	TeamAIControllerRef->UseBlackboard(BlackboardDataRef, BlackboardComponentRef);
	if (!BehaviorTreeRef) { return; }
	TeamAIControllerRef->RunBehaviorTree(BehaviorTreeRef);
}

bool ABasicRobotCharacter::SetLookRotation(FRotator NewRotation, const bool DoReset)
{
	if (DoReset) {
		IsLookRotating	= true;
		UseLookRotation	= false;
		return true;
	}
	FRotator RelativeRotation = GetActorRotation() - NewRotation;
	if (RelativeRotation.Yaw >= LookRotationYawLimit.X && RelativeRotation.Yaw <= LookRotationYawLimit.Y) {
		// Looking in this direction will require turning the actor.
	}
	RelativeRotation.Yaw	= FMath::Clamp(RelativeRotation.Yaw, LookRotationYawLimit.X, LookRotationYawLimit.Y);
	RelativeRotation.Pitch	= FMath::Clamp(RelativeRotation.Pitch, LookRotationPitchLimit.X, LookRotationPitchLimit.Y);
	
	LookRotation	= GetActorRotation() + RelativeRotation;
	IsLookRotating	= true;
	UseLookRotation	= true;
	return true;
	//}
	//return false;
}

void ABasicRobotCharacter::OnBehaviorStateChanged_Implementation()
{
	switch (CurrentBehaviorState) {
		case EAIBehaviorState::BS_PATROL:
			SetLightColor(FLinearColor(1.f, 1.f, 0));
			break;
		case EAIBehaviorState::BS_SEARCH:
			SetLightColor(FLinearColor(1.f, 1.f, 0));
			break;
		case EAIBehaviorState::BS_HUNT:
			SetLightColor(FLinearColor(1.f, 0, 0));
			break;
		case EAIBehaviorState::BS_HUNT_SEARCH:
			SetLightColor(FLinearColor(1.f, 0.4f, 0));
			break;
		case EAIBehaviorState::BS_IDLE:
		default:
			SetLightColor(FLinearColor::White);
			break;
	}
}

void ABasicRobotCharacter::SetLightColor(FLinearColor color)
{
	CameraLight->SetLightColor(color);
	CameraAccentLight->SetLightColor(color);
}

void ABasicRobotCharacter::UpdateLookTarget(AActor* NewTarget)
{
	if (IsValid(NewTarget)) {
		FRotator NewLookRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), NewTarget->GetActorLocation());
		FRotator RelativeRotation = GetActorRotation() - NewLookRotation;
		if (RelativeRotation.Yaw >= LookRotationYawLimit.X && RelativeRotation.Yaw <= LookRotationYawLimit.Y && RelativeRotation.Pitch >= LookRotationPitchLimit.X && RelativeRotation.Pitch <= LookRotationPitchLimit.Y) {
			SetLookRotation(NewLookRotation);
		}
		else {
			return;
		}
	}
	LookTargetRef = NewTarget;
	if (IsValid(LookTargetRef)) {
		switch (TeamAIControllerRef->GetTeamAttitudeTowards(*LookTargetRef)) {
		case ETeamAttitude::Friendly:
			SetLightColor(FLinearColor(255, 255, 255));
			break;
		case ETeamAttitude::Hostile:
			SetLightColor(FLinearColor(255, 0, 0));
			break;
		case ETeamAttitude::Neutral:
			SetLightColor(FLinearColor(0, 255, 255));
			break;
		default:
			SetLightColor(FLinearColor(255, 255, 255));
			break;
		}
	}
	else {
		SetLightColor(FLinearColor(255, 255, 255));
		LookRotation = GetActorRotation();
		IsLookRotating = true;
	}
}

void ABasicRobotCharacter::OnTargetPerceptionInfoUpdated_Implementation(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	Super::OnTargetPerceptionInfoUpdated_Implementation(UpdateInfo);
	return;
	if (IsValid(UpdateInfo.Target.Get())) {
		if (UpdateInfo.Stimulus.WasSuccessfullySensed() && ETeamAttitude::Hostile == TeamAIControllerRef->GetTeamAttitudeTowards(*UpdateInfo.Target.Get())) {
			UpdateLookTarget(UpdateInfo.Target.Get());
		}
	}
}

FVector ABasicRobotCharacter::GetActorEyesLocation_Implementation() const
{
	return CameraLookOverride->GetComponentLocation();
}

FRotator ABasicRobotCharacter::GetActorEyesRotation_Implementation() const
{
	return CameraLookOverride->GetComponentRotation();
}
