// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalGameEditor.h"
#include "UnrealEd.h"
#include "PatrolPathVisualizer.h"
#include "AI/PatrolNodeComponent.h"

IMPLEMENT_GAME_MODULE(FSurvivalGameEditorModule, SurvivalGameEditor)

void FSurvivalGameEditorModule::StartupModule()
{
	if (GUnrealEd)
	{
		TSharedPtr<FPatrolPathVisualizer> Visualizer = MakeShareable(new FPatrolPathVisualizer());
		GUnrealEd->RegisterComponentVisualizer(UPatrolNodeComponent::StaticClass()->GetFName(), Visualizer);
		Visualizer->OnRegister();
	}
}

void FSurvivalGameEditorModule::ShutdownModule()
{
	if (GUnrealEd)
	{
		GUnrealEd->UnregisterComponentVisualizer(UPatrolNodeComponent::StaticClass()->GetFName());
	}
}