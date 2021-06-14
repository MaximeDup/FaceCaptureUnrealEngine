// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;

public class AndroidFaceCaptureEditor : ModuleRules
{
	public AndroidFaceCaptureEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	

        
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                
                    "Core",
                    "CoreUObject",
                    "AndroidFaceCapture",
                    "UnrealEd", // for FAssetEditorManager
            }
            );
        
        PrivateDependencyModuleNames.AddRange(
            new string[] 
            { 
               // "ImageWriteQueue", 
               // "PhysicsCore",
               // "RenderCore",
                 "SlateCore",
                 "Projects",
                 "EditorStyle",
            }
            );

    }
}

