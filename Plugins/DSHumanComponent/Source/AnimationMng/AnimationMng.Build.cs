// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System;
public class AnimationMng : ModuleRules
{
	public AnimationMng(ReadOnlyTargetRules Target) : base(Target)
	{
		 PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;


        PublicIncludePaths.AddRange(
            new string[] {
                System.IO.Path.Combine(ModuleDirectory,"Public")

			}
            );


        PrivateIncludePaths.AddRange(
            new string[] {
                System.IO.Path.Combine(ModuleDirectory,"Private"),
			}
            );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "AnimGraphRuntime",
                "AnimationCore",
                "DSHumanComponent",
                "LiveLink",
                "LiveLinkInterface",
                "LiveLinkAnimationCore",
                
			}
            );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                
				// ... add private dependencies that you statically link with here ...	
			}
            );
	}
}
