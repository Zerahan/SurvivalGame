// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/GameTeams.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "SurvivalAICharacter.generated.h"

UENUM(BlueprintType)
enum class EAIBehaviorState : uint8
{
	BS_IDLE			UMETA(DisplayName = "Idle"),
	BS_SEARCH		UMETA(DisplayName = "Search"),
	BS_PATROL		UMETA(DisplayName = "Patrol"),
	BS_HUNT			UMETA(DisplayName = "Hunt"),
	BS_HUNT_SEARCH	UMETA(DisplayName = "Hunt Search"),
};


UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom))
class SURVIVALGAME_API ASurvivalAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASurvivalAICharacter();

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, Category = "AI", meta = (BlueprintProtected))
	class UAIPerceptionComponent* AIPerceptionComponentRef;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "AI", meta = (BlueprintProtected))
	class ATeamAIController* TeamAIControllerRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI", meta = (BlueprintProtected))
	EGameTeam TeamID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI", meta = (BlueprintProtected))
	class UAISenseConfig_Sight* AIPerceptionConfig_Sight;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI", meta = (BlueprintProtected))
	class UAISenseConfig_Hearing* AIPerceptionConfig_Hearing;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI", meta = (BlueprintProtected))
	float SightRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI", meta = (BlueprintProtected))
	float SightRange_MAX;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI", meta = (BlueprintProtected))
	float SightAngle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI", meta = (BlueprintProtected))
	float SightMaxAge;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI", meta = (BlueprintProtected))
	float HearingRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI", meta = (BlueprintProtected))
	float HearingMaxAge;

	UPROPERTY(BlueprintReadOnly, Category = "AI|Behavior", meta = (BlueprintProtected))
	TArray<AActor*> PerceptionPriorityList;

	UPROPERTY(BlueprintReadOnly, Category = "AI|Behavior", meta = (BlueprintProtected))
	AActor* LastTopPerceptionRef;

	UPROPERTY(BlueprintReadOnly, Category = "AI|Behavior", meta = (BlueprintProtected))
	int32 LastSensedStimIndex;

	UPROPERTY(BlueprintReadOnly, Category = "AI|Behavior", meta = (BlueprintProtected))
	EAIBehaviorState CurrentBehaviorState;

	UPROPERTY(BlueprintReadOnly, Category = "AI|Behavior", meta = (BlueprintProtected))
	EAIBehaviorState LastBehaviorState;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewTeamAIControllerRef) override;

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetSightRange(const float NewSightRange = 3000.f, const float NewSightRange_MAX = 3500.f);
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetSightAngle(const float NewSightAngle = 90.f);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI")
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	virtual void OnPerceptionUpdated_Implementation(const TArray<AActor*>& UpdatedActors) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI")
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	virtual void OnTargetPerceptionUpdated_Implementation(AActor* Actor, FAIStimulus Stimulus) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI")
	void OnTargetPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo);
	virtual void OnTargetPerceptionInfoUpdated_Implementation(const FActorPerceptionUpdateInfo& UpdateInfo);

	UFUNCTION(BlueprintCallable, Category = "AI|Behavior")
	void SetBehaviorState(const EAIBehaviorState NewBehaviorState);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI|Behavior")
	void OnBehaviorStateChanged();
	virtual void OnBehaviorStateChanged_Implementation() {}

	UFUNCTION(BlueprintCallable, Category = "AI|Behavior")
	TArray<AActor*> GetPerceptionPriorityList() const;

	UFUNCTION(BlueprintCallable, Category = "AI|Behavior")
	void ExpireLatestForTopPerception();

	UFUNCTION(BlueprintCallable, Category = "AI|Behavior")
	AActor* GetPerceptionTopPriority(FVector& LastLocation, bool& IsLocationValid);

	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI")
	FVector GetActorEyesLocation() const;
	virtual FVector GetActorEyesLocation_Implementation() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI")
	FRotator GetActorEyesRotation() const;
	virtual FRotator GetActorEyesRotation_Implementation() const;
};
