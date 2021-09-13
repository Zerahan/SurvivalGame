// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Characters/GameTeams.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "SurvivalAIPawn.generated.h"

class UAIPerceptionComponent;
class ATeamAIController;
class UAISenseConfig_Sight;

UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom))
class SURVIVALGAME_API ASurvivalAIPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASurvivalAIPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "AI", meta = (BlueprintProtected))
	UAIPerceptionComponent* AIPerceptionComponentRef;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "AI", meta = (BlueprintProtected))
	ATeamAIController* TeamAIControllerRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI", meta = (BlueprintProtected))
	EGameTeam TeamID;

	UAISenseConfig_Sight* AIPerceptionConfig_Sight;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI", meta = (BlueprintProtected))
	float SightRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI", meta = (BlueprintProtected))
	float SightRange_MAX;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI", meta = (BlueprintProtected))
	float SightAngle;

	UPROPERTY(BlueprintReadOnly, Category = "AI|Behavior", meta = (BlueprintProtected))
	TArray<AActor*> PerceptionPriorityList;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewTeamAIControllerRef) override;

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetSightRange(float NewSightRange = 3000.f, float NewSightRange_MAX = 3500.f);

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetSightAngle(float NewSightAngle = 90.f);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI")
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	void OnPerceptionUpdated_Implementation(const TArray<AActor*>& UpdatedActors) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI")
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	void OnTargetPerceptionUpdated_Implementation(AActor* Actor, FAIStimulus Stimulus) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI")
	void OnTargetPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo);
	void OnTargetPerceptionInfoUpdated_Implementation(const FActorPerceptionUpdateInfo& UpdateInfo) {}

	UFUNCTION(BlueprintCallable, Category = "AI|Behavior")
	TArray<AActor*> GetPerceptionPriorityList() const;

	UFUNCTION(BlueprintCallable, Category = "AI|Behavior")
	AActor* GetPerceptionTopPriority() const;

	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI")
	FVector GetActorEyesLocation() const;
	FVector GetActorEyesLocation_Implementation() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI")
	FRotator GetActorEyesRotation() const;
	FRotator GetActorEyesRotation_Implementation() const;
};
