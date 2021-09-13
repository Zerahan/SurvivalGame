// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PatrolPath.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Texture2D.h"
#include "Components/CapsuleComponent.h"
#include "Components/BillboardComponent.h"
#include "Components/TimelineComponent.h"
#include "AI/PatrolNodeComponent.h"
#include "AI/PatrolNode.h"


APatrolPath::APatrolPath()
{
#if WITH_EDITORONLY_DATA
	PatrolVisualizer = CreateEditorOnlyDefaultSubobject<UPatrolNodeComponent>(TEXT("PatrolVisualizer"));
	BillboardComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));

	if (!IsRunningCommandlet())
	{
		// Structure to hold one-time initialization
		struct FConstructorStatics
		{
			ConstructorHelpers::FObjectFinderOptional<UTexture2D> PatrolPathTextureObject;
			FName ID_PatrolPath;
			FText NAME_PatrolPath;
			FName ID_Navigation;
			FText NAME_Navigation;
			FConstructorStatics()
				: PatrolPathTextureObject(TEXT("/Engine/EditorResources/Goal_Waypoint"))
				, ID_PatrolPath(TEXT("PatrolPath"))
				, NAME_PatrolPath(NSLOCTEXT("SpriteCategory", "PatrolPath", "Patrol Path"))
				, ID_Navigation(TEXT("Navigation"))
				, NAME_Navigation(NSLOCTEXT("SpriteCategory", "Navigation", "Navigation"))
			{
			}
		};
		static FConstructorStatics ConstructorStatics;
		
		if (BillboardComponent) {
			BillboardComponent->Sprite = ConstructorStatics.PatrolPathTextureObject.Get();
			BillboardComponent->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
			BillboardComponent->SpriteInfo.Category = ConstructorStatics.ID_PatrolPath;
			BillboardComponent->SpriteInfo.DisplayName = ConstructorStatics.NAME_PatrolPath;
		}
	}
#endif // WITH_EDITORONLY_DATA
}

int32 APatrolPath::GetNextNodeID(const int32 CurrentIndex, AActor* AskingActor, const ETimelineDirection::Type Direction) const
{
	if (!Nodes.IsValidIndex(CurrentIndex)) {
		return 0;
	}
	if (!IsNodeClaimed(CurrentIndex, AskingActor)) {
		return CurrentIndex;
	}
	return GetNextNodeID_IgnoreClaim(CurrentIndex, Direction);
}

int32 APatrolPath::GetNextNodeID_IgnoreClaim(const int32 CurrentIndex, const ETimelineDirection::Type Direction) const
{
	if (Direction == ETimelineDirection::Forward) {
		if (CurrentIndex + 1 < Nodes.Num()) {
			return CurrentIndex + 1;
		}
		else {
			return 0;
		}
	}
	else {
		if (CurrentIndex - 1 > 0) {
			return CurrentIndex - 1;
		}
		else {
			return Nodes.Num() - 1;
		}
	}
}

APatrolNode* APatrolPath::GetNode(const int32 Index, AActor* AskingActor) const
{
	if (Nodes.IsValidIndex(Index)) {
		if (IsValid(Nodes[Index])) {
			return Nodes[Index];
		}
	}
	return nullptr;
}

FVector APatrolPath::GetNodeIDWorldLocation(const int32 Index, AActor* AskingActor) const
{
	if (Nodes.IsValidIndex(Index)) {
		if (IsValid(Nodes[Index])) {
			return Nodes[Index]->GetActorLocation();
		}
	}
	return FVector();
}

bool APatrolPath::IsNodeClaimed(const int32 Index, AActor* AskingActor) const
{
	if (!Nodes.IsValidIndex(Index)) { return false; }
	return Nodes[Index]->IsClaimed(AskingActor);
}

