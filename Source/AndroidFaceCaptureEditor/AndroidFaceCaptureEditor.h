// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "AssetToolsModule.h"
#include "AssetTypeCategories.h"

class FAndroidFaceCaptureEditor : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	static EAssetTypeCategories::Type GetAssetCategory() { return FaceAndroidAssetCategory; }

public:

	TSharedPtr<FSlateStyleSet> StyleSet;

	static EAssetTypeCategories::Type FaceAndroidAssetCategory;
	TArray<TSharedPtr<IAssetTypeActions>> CreatedAssetTypeActions;
};

