using UnrealBuildTool;

public class DUETween : ModuleRules
{
	public DUETween(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "Engine", "DeveloperSettings", "CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UMG"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{

			}
		);
	}
}