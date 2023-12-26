// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System;
public class AnimationMngEditor : ModuleRules
{
    public AnimationMngEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
                System.IO.Path.Combine(ModuleDirectory,"Public")
				// ... add public include paths required here ...
			}
            );


        PrivateIncludePaths.AddRange(
            new string[] {
                System.IO.Path.Combine(ModuleDirectory,"Private")
				// ... add other private include paths required here ...
			}
            );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "AnimationMng",
                "LiveLinkInterface",
                "InputCore",

			}
            );

        if(Target.Version.MinorVersion>2)
        {
            PublicDependencyModuleNames.Add("Persona");
        }

        if (Target.Type == TargetRules.TargetType.Editor)
        {
            PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "AnimGraph",
                "BlueprintGraph",
                "UnrealEd",
                "EditorFramework",
			}
            );

            if (Target.Version.MinorVersion > 0)
            {
                PrivateDependencyModuleNames.Add("AnimationEditMode");
            }
            else
            {
                PrivateDependencyModuleNames.AddRange(
                new string[]
                {"EditorInteractiveToolsFramework", "InteractiveToolsFramework"});
            }

        }


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "SlateCore",
            }
            );

    }
}
