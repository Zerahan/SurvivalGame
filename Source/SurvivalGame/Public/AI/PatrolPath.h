// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "PatrolPath.generated.h"

class APatrolNode;

UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom))
class SURVIVALGAME_API APatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	APatrolPath();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<APatrolNode*> Nodes;
	
#if WITH_EDITORONLY_DATA
private:
	UPROPERTY()
	class UPatrolNodeComponent* PatrolVisualizer;

	UPROPERTY()
	class UBillboardComponent* BillboardComponent;
#endif

public:
	UFUNCTION(BlueprintCallable, Category="AI|PatrolPath")
	int32 GetNextNodeID(const int32 CurrentIndex, AActor* AskingActor = nullptr, const ETimelineDirection::Type Direction = ETimelineDirection::Forward) const;

	UFUNCTION(BlueprintCallable, Category = "AI|PatrolPath")
	int32 GetNextNodeID_IgnoreClaim(const int32 CurrentIndex, const ETimelineDirection::Type Direction = ETimelineDirection::Forward) const;

	UFUNCTION(BlueprintCallable, Category = "AI|PatrolPath")
	APatrolNode* GetNode(const int32 Index, AActor* AskingActor = nullptr) const;

	UFUNCTION(BlueprintCallable, Category = "AI|PatrolPath")
	FVector GetNodeIDWorldLocation(const int32 Index, AActor* AskingActor = nullptr) const;

	UFUNCTION(BlueprintCallable, Category = "AI|PatrolPath")
	bool IsNodeClaimed(const int32 Index, AActor* AskingActor = nullptr) const;
};
