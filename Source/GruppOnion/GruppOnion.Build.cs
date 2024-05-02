// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
public class GruppOnion : ModuleRules
{
	public GruppOnion(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "ProceduralMeshComponent", "Landscape" });

		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule", "GameplayTasks", "NavigationSystem"});
	}
}
