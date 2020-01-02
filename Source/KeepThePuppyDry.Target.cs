// Copyright 2020, Kevin VanHorn. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class KeepThePuppyDryTarget : TargetRules
{
	public KeepThePuppyDryTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "KeepThePuppyDry" } );
    }
}
