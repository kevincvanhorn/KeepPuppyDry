// Copyright 2020, Kevin VanHorn. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class KeepThePuppyDryTarget : TargetRules
{
	public KeepThePuppyDryTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "KeepThePuppyDry" } );

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            ExtraModuleNames.Add("OnlineSubsystemGooglePlay");
            ExtraModuleNames.Add("OnlineSubsystem");
            ExtraModuleNames.Add("AndroidAdvertising");
        }
    }
}
