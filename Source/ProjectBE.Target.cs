// Copyright (C) 2023 owoDra

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectBETarget : TargetRules
{
	public ProjectBETarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("ProjectBE");
	}
}
