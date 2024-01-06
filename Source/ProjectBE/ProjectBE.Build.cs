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
                "Core", "CoreUObject", "Engine",

                "Slate", "SlateCore", "RenderCore",

                "AIModule", "ApplicationCore", "PhysicsCore",

                "InputCore", "EnhancedInput",

                "AudioMixer", "AudioModulation",

                "CommonUI", "CommonInput",

                "GameplayTags", "GameplayAbilities", "ModularGameplay",
            }
        );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                // Game XXX Core
                "GFCore", "GSCore", "GACore",

                // Game XXX Extension
                "GCExt", "GAExt", "GEExt",
                "GIExt", "GTExt", "GUIExt",
                "GVExt", "GFXExt",

                // Game XXX: YYY Addon
                "GAHAddon",
                "GAEAddon", "GEMAddon",
                "GCLAddon", "GLHAddon",

                // Game XXX: YYY Integration
                "GCAIntg", "GCEIntg", "GCHIntg", 
                "GCIIntg", "GCVIntg", "GCFXIntg",
                "GASIntg", "GAUIIntg",
                "GIAIntg", "GLAIntg",
                "GSIIntg",
                "GHFXIntg",
            }
        );

        SetupIrisSupport(Target);
    }
}
