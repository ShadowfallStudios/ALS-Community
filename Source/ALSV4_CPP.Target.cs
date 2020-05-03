// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ALSV4_CPPTarget : TargetRules
{
	public ALSV4_CPPTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		ExtraModuleNames.AddRange(new string[] {"ALSV4_CPP"});

		bIWYU = true;
		bLegacyPublicIncludePaths = false;
	}
}