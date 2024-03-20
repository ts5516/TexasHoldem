// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TH : ModuleRules
{
	public TH(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"HeadMountedDisplay",
			"EnhancedInput",
            "OnlineSubsystem",
			"OnlineSubsystemUtils",
        });
	}
}
