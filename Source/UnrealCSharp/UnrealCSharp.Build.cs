// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using System.Text.RegularExpressions;

public class UnrealCSharp : ModuleRules
{
	public UnrealCSharp(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		// Some Engine forks, including Hazelight, already provide these types.
		// Detect the actual header rather than attributing fork additions to UE 5.8.
		var CoreClassHeader = Path.Combine(EngineDirectory, "Source", "Runtime", "CoreUObject", "Public", "UObject", "Class.h");
		ExternalDependencies.Add(CoreClassHeader);
		var CoreClassSource = File.ReadAllText(CoreClassHeader);
		foreach (var StructName in new[] { "FBox", "FBoxSphereBounds" })
		{
			var HasSpecialization = Regex.IsMatch(CoreClassSource, @"template\s*<>\s*struct\s+TBaseStructure\s*<\s*" + StructName + @"\s*>");
			PublicDefinitions.Add("UNREALCSHARP_ENGINE_BASE_" + StructName.ToUpperInvariant() + "=" + (HasSpecialization ? "1" : "0"));
		}
		
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
				"Engine",
				// ... add other public dependencies that you statically link with here ...
				"CrossVersion",
				"UnrealCSharpCore"
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Slate",
				"SlateCore",
				"UMG",
				"InputCore",
				"EnhancedInput"
				// ... add private dependencies that you statically link with here ...	
			}
			);

		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"UnrealEd",
				}
			);
		}
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
