// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyContentExample : ModuleRules
{
	public MyContentExample(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateIncludePaths.Add("MyContentExample/");
		
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput",
		});
		
		
		// Gameplay Ability(GAS)
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks", 
			"MotionWarping"
		});
	}
}
