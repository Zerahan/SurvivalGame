// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SurvivalGame.h"
#include "Engine/NavigationObjectBase.h"
#include "PatrolNode.generated.h"

UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom))
class SURVIVALGAME_API APatrolNode : public ANavigationObjectBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APatrolNode(const FObjectInitializer& ObjectInitializer);

	AActor* OccupyingEntityRef;

	/** Arrow component to indicate forward direction of start */
#if WITH_EDITORONLY_DATA
private:
	UPROPERTY()
	class UArrowComponent* ArrowComponent;
#endif

#if WITH_EDITORONLY_DATA
public:
	/** Returns ArrowComponent subobject **/
	class UArrowComponent* GetArrowComponent() const;
#endif

public:
	UFUNCTION(BlueprintCallable, Category = "AI|Patrol")
	bool IsClaimed(AActor* AskingActorRef = nullptr) const;

	UFUNCTION(BlueprintCallable, Category = "AI|Patrol")
	bool CanClaimNode(AActor* ClaimingActorRef) const;
	
	UFUNCTION(BlueprintCallable, Category="AI|Patrol")
	bool ClaimNode(AActor* ClaimingActorRef);

	UFUNCTION(BlueprintCallable, Category = "AI|Patrol")
	bool LeaveNode(AActor* LeavingActorRef);
};
