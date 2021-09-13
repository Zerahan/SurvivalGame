// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPathVisualizer.h"
#include "AI/PatrolPath.h"
#include "AI/PatrolNodeComponent.h"
#include "AI/PatrolNode.h"
#include "Components/CapsuleComponent.h"

void FPatrolPathVisualizer::DrawVisualization(const UActorComponent* ComponentRef, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	const APatrolPath* PatrolPath = Cast<const APatrolPath>(ComponentRef->GetOwner());

	int32 NumNodes = PatrolPath->Nodes.Num();
	int32 NextNodeIndex;
	float Percent;
	if (NumNodes > 1) {
		for (int i = 0; i < NumNodes; i++) {
			Percent = (float)i / ((float)NumNodes-1);
			NextNodeIndex = PatrolPath->GetNextNodeID_IgnoreClaim(i);
			if (IsValid(PatrolPath->Nodes[i]) && IsValid(PatrolPath->Nodes[NextNodeIndex])) {
				PDI->DrawLine(
					PatrolPath->Nodes[i]->GetActorLocation(),
					PatrolPath->Nodes[NextNodeIndex]->GetActorLocation(),
					FLinearColor::LerpUsingHSV(FLinearColor(0.f, 1.f, 1.f), FLinearColor(1.f, 0.f, 0.f), Percent),
					SDPG_World,
					2.f
				);
				
				DrawWireSphere(PDI,
					PatrolPath->Nodes[i]->GetActorLocation(),
					FLinearColor::LerpUsingHSV(FLinearColor(0.f, 1.f, 1.f), FLinearColor(1.f, 0.f, 0.f), Percent),
					16,	//Radius
					16,	//Sides
					SDPG_Foreground,
					2.f //Thickness
				);
			}
		}
	}

	/*
	if (IsValid(PatrolPath->NextNode)) {
		PDI->DrawLine(
			PatrolPath->GetActorLocation(),
			PatrolPath->NextNode->GetActorLocation(),
			FLinearColor(0.f, 1.f, 1.f),
			SDPG_World,
			2.f
		);
	}

	if (IsValid(PatrolPath->PreviousNode)) {
		PDI->DrawLine(
			PatrolPath->GetActorLocation(),
			PatrolPath->PreviousNode->GetActorLocation(),
			FLinearColor(1.f, 0.f, 0.f),
			SDPG_World,
			2.f
		);
	}
	//*/
}
