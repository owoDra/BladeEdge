// Copyright (C) 2024 owoDra

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectBE_LANTarget : ProjectBETarget
{
	public ProjectBE_LANTarget(TargetInfo Target) : base(Target)
	{
		// Add override directory for LAN config files
		// We have to recompile the engine to add the custom config preprocessor definition, but that is currently not possible for installed builds
		
		CustomConfig = "LAN";
	}
}
