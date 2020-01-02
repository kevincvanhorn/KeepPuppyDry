// Copyright 2020, Kevin VanHorn. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class KeepThePuppyDryEditorTarget : TargetRules
{
	public KeepThePuppyDryEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "KeepThePuppyDry" } );
	}
}
