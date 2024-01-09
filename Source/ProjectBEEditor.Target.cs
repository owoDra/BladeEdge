// Copyright (C) 2024 owoDra

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectBEEditorTarget : TargetRules
{
	public ProjectBEEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("ProjectBE");
	}
}
