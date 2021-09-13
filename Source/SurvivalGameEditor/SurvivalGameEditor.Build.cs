// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SurvivalGameEditor : ModuleRules
{
	public SurvivalGameEditor( ReadOnlyTargetRules Target ) : base( Target ) {
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange( new string[] { "Core", "CoreUObject", "Engine", "SurvivalGame" } );

		PrivateDependencyModuleNames.AddRange( new string[] { "UnrealEd" } );
	}
}
