// Copyright (C) 2023 owoDra

using UnrealBuildTool;

public class ProjectBE : ModuleRules
{
	public ProjectBE(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[]
            {
                ModuleDirectory,
            }
        );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "AIModule",
            }
        );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                // Game XXX Core
                "GFCore", "GSCore",

                // Game XXX Extension
                "GCExt", "GAExt", "GEExt",
                "GIExt", "GTExt", "GUIExt",

                // Game XXX: YYY Addon
                "GAHAddon",
                "GAEAddon", "GEMAddon",
                "GCLAddon", "GLHAddon",

                // Game XXX: YYY Integration
                "GCAIntg", "GCEIntg", "GCHIntg", "GCIIntg",
                "GIAIntg", "GLAIntg",
                "GSIIntg",
            }
        );

        SetupIrisSupport(Target);
    }
}
