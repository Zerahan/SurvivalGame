// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComponentVisualizer.h"

/**
 * 
 */
class SURVIVALGAMEEDITOR_API FPatrolPathVisualizer: public FComponentVisualizer
{
private:
	virtual void DrawVisualization(const UActorComponent* ComponentRef, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
};
