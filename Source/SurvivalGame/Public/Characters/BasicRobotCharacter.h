// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SurvivalGame.h"
#include "Characters/SurvivalAICharacter.h"
#include "BasicRobotCharacter.generated.h"

class APatrolPath;
class UBehaviorTree;
class UBlackboardComponent;
class UBlackboardComponent;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom))
class SURVIVALGAME_API ABasicRobotCharacter : public ASurvivalAICharacter
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USceneComponent* CameraMount;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USceneComponent* CameraLookOverride;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* CameraMesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class USpotLightComponent* CameraLight;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UPointLightComponent* CameraAccentLight;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI|Behavior")
	FVector2D LookRotationYawLimit;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI|Behavior")
	FVector2D LookRotationPitchLimit;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI|Patrol")
	APatrolPath* PatrolPathRef;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Behavior")
	UBehaviorTree* BehaviorTreeRef;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Behavior")
	UBlackboardData* BlackboardDataRef;

	UPROPERTY()
	UBlackboardComponent* BlackboardComponentRef;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "AI|Behavior")
	AActor* LookTargetRef;

	UPROPERTY(BlueprintReadOnly, Category = "AI|Behavior")
	bool IsLookRotating;

	UPROPERTY(BlueprintReadOnly, Category = "AI|Behavior")
	bool UseLookRotation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI|Behavior")
	FRotator LookRotation;

public:
	// Sets default values for this character's properties
	ABasicRobotCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewTeamAIControllerRef) override;

	UFUNCTION(BlueprintCallable, Category = "AI|Behavior")
	bool SetLookRotation(FRotator NewRotation, const bool DoReset = false);

	virtual void OnBehaviorStateChanged_Implementation();
	
	UFUNCTION(BlueprintCallable)
	void SetLightColor(FLinearColor color);

	UFUNCTION(BlueprintCallable, Category = "AI|Behavior")
	void UpdateLookTarget(AActor* NewTarget);

	virtual void OnPerceptionUpdated_Implementation(const TArray<AActor*>& UpdatedActors) override {}
	virtual void OnTargetPerceptionUpdated_Implementation(AActor* Actor, FAIStimulus Stimulus) override {}
	virtual void OnTargetPerceptionInfoUpdated_Implementation(const FActorPerceptionUpdateInfo& UpdateInfo) override;

	virtual FVector GetActorEyesLocation_Implementation() const override;
	virtual FRotator GetActorEyesRotation_Implementation() const override;
};
