// Copyright:       Copyright (C) 2022 Doğa Can Yanıkoğlu
// Source Code:     https://github.com/dyanikoglu/ALS-Community

using UnrealBuildTool;

public class ALSV4_CPP : ModuleRules
{
	public ALSV4_CPP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
			{"Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "GameplayTasks","PhysicsCore", "Niagara", "EnhancedInput"
			});

		PrivateDependencyModuleNames.AddRange(new[] {"Slate", "SlateCore"});
	}
}