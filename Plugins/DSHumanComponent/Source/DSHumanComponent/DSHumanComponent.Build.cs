// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class DSHumanComponent : ModuleRules
{
    private string ModulePath
    {
        get
        {
            return ModuleDirectory;
        }

    }

    private void AddDep(string FileName, string Ext, string SourceDir, string TargetDir)
    {
        RuntimeDependencies.Add(Path.Combine(TargetDir, FileName + Ext), Path.Combine(SourceDir, FileName + Ext));
    }



    private string ThirdPartyPath
    {
        get
        {
            return Path.GetFullPath(Path.Combine(ModulePath, "ThirdParty"));
        }
    }

    public DSHumanComponent(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        ShadowVariableWarningLevel = WarningLevel.Off;
        bEnableUndefinedIdentifierWarnings = false;

        PublicIncludePaths.AddRange(
            new string[] {
				// ... add public include paths required here ...
			}
            );


        PrivateIncludePaths.AddRange(
            new string[] {
				// ... add other private include paths required here ...
			}
            );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "Sockets",
            }
            );

        if (Target.Version.MinorVersion == 0)
        {
            PublicDependencyModuleNames.Add("Networking");
        }
        


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "DeveloperSettings",
                "Slate",
                "SlateCore",
                "UMG",
                "Projects",
			}
            );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
				// ... add any modules that your module loads dynamically here ...
			}
            );

        string BinaryOutputDir = "$(BinaryOutputDir)";
        string ThirdPartyDir = Path.Combine(ModuleDirectory, "ThirdParty");
        string MPBinDir = Path.Combine(ThirdPartyDir, "Binaries", "Win64");

        AddDep("DSCapture", ".dll", MPBinDir, BinaryOutputDir);
        //AddDep("jsoncpp", ".dll", MPBinDir, BinaryOutputDir);

        //PublicIncludePaths.Add(Path.Combine(ThirdPartyDir, "include"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyDir, "lib", "HttpClient.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyDir, "lib", "kcpnet.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyDir, "lib", "jsoncpp.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyDir, "lib", "libprotobuf.lib"));



        //RuntimeDependencies.Add(Path.Combine(TargetDir, FileName + Ext), Path.Combine(SourceDir, FileName + Ext));
        /*RuntimeDependencies.Add(Path.Combine(ThirdPartyPath, "Binaries/Win64", "opencv_world346.dll"));
        //PublicIncludePaths为 通向此模块内部包含文件的所有路径的列表，不向其他模块公开(至少有一个包含到Private 路径，如果要避免相对路径，则会更多)
        //PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "include"));

        //附加库列表,PublicAdditionalLibraries是一组包含其他预编译库的列表（.lib文件的名称列表，包含后缀）...
        PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "glfw3.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "glad.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "opencv_world346.lib"));*/

    }
}
