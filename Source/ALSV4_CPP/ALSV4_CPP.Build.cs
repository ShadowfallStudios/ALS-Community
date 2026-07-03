// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ALSV4_CPP : ModuleRules
{
	public ALSV4_CPP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
				"ALSV4_CPP/Public",
				"ALSV4_CPP/Public/Character",
				"ALSV4_CPP/Public/Weapon",
				"ALSV4_CPP/Public/Components",
				"ALSV4_CPP/Public/AI",
				"ALSV4_CPP/Public/Library"
			}
		);

		PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"EnhancedInput"
			}
		);
	}
}
