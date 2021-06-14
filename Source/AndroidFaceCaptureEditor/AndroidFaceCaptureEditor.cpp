// Copyright Epic Games, Inc. All Rights Reserved.

#include "AndroidFaceCaptureEditor.h"
//#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"

#include "AndroidBlenshapeAssetTypeActions.h"
#include "Internationalization/Internationalization.h"



IMPLEMENT_PRIMARY_GAME_MODULE(FAndroidFaceCaptureEditor, AndroidFaceCaptureEditor, "AndroidFaceCaptureEditor" );

#define LOCTEXT_NAMESPACE "AndroidBlenshape"

EAssetTypeCategories::Type FAndroidFaceCaptureEditor::FaceAndroidAssetCategory;

void FAndroidFaceCaptureEditor::StartupModule()
{	

	// Only register once
	if (!StyleSet.IsValid())
	{	
		StyleSet = MakeShareable(new FSlateStyleSet("FaceAndroidStyle"));
	
		FString ContentDir = FPaths::Combine(FPaths::ProjectDir(), TEXT("Resources"));
		StyleSet->SetContentRoot(ContentDir);

		FSlateImageBrush* IconBrush = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("face-scan_16"), TEXT(".png")), FVector2D(16.f, 16.f));
		FSlateImageBrush* ThumbnailBrush = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("face-scan_128"), TEXT(".png")), FVector2D(128.f, 128.f));

		if (IconBrush)
			StyleSet->Set("ClassIcon.AndroidBlenshape", IconBrush);
		if (ThumbnailBrush)
			StyleSet->Set("ClassThumbnail.AndroidBlenshape", ThumbnailBrush);

		FSlateImageBrush* IconBrush_mesh = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("face-scan2_16"), TEXT(".png")), FVector2D(16.f, 16.f));
		FSlateImageBrush* ThumbnailBrush_mesh = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("face-scan2_128"), TEXT(".png")), FVector2D(128.f, 128.f));


		if (IconBrush_mesh)
			StyleSet->Set("ClassIcon.AndroidBlenshapeWeights", IconBrush_mesh);
		if (ThumbnailBrush_mesh)
			StyleSet->Set("ClassThumbnail.AndroidBlenshapeWeights", ThumbnailBrush_mesh);
/*
		//Create a brush from the icon

		


		FSlateImageBrush* IconBrush_mat = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("Icon16_mat"), TEXT(".png")), FVector2D(16.f, 16.f));
		FSlateImageBrush* ThumbnailBrush_mat = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("Icon128_mat"), TEXT(".png")), FVector2D(128.f, 128.f));

		if (IconBrush_mat)
			StyleSet->Set("ClassIcon.PlanetGroundMaterial", IconBrush_mat);
		if (ThumbnailBrush_mat)
			StyleSet->Set("ClassThumbnail.PlanetGroundMaterial", ThumbnailBrush_mat);

		FSlateImageBrush* IconBrush_PData = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("Icon16_Pdata"), TEXT(".png")), FVector2D(16.f, 16.f));
		FSlateImageBrush* ThumbnailBrush_PData = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("Icon128_Pdata"), TEXT(".png")), FVector2D(128.f, 128.f));

		if (IconBrush_PData)
			StyleSet->Set("ClassIcon.PlanetStaticEcoData", IconBrush_PData);
		if (ThumbnailBrush_PData)
			StyleSet->Set("ClassThumbnail.PlanetStaticEcoData", ThumbnailBrush_PData);
		
*/
		FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());

	}




	// Register asset types
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	FaceAndroidAssetCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("FaceAndroid")), LOCTEXT("FaceAndroidAssetCategory", "FaceAndroid"));

	// Register asset type actions for DatasmithScene class	
	//TSharedPtr<FEcoSystemAssetTypeActions> EcoSystemAssetTypeAction = MakeShared<FEcoSystemAssetTypeActions>();
	//AssetTools.RegisterAssetTypeActions(EcoSystemAssetTypeAction.ToSharedRef());
	
	// Helper lambda for registering asset type actions for automatic cleanup on shutdown
	auto RegisterAssetTypeAction = [&](TSharedRef<IAssetTypeActions> Action)
	{
		AssetTools.RegisterAssetTypeActions(Action);
		CreatedAssetTypeActions.Add(Action);
	};

	// Register type actions
	RegisterAssetTypeAction(MakeShareable(new FAndroidBlenshapeAssetTypeActions));
	RegisterAssetTypeAction(MakeShareable(new FAndroidBlenshapeWeightsAssetTypeActions));
	/*
	

	RegisterAssetTypeAction(MakeShareable(new FPlanetGroundMaterialAssetTypeActions));
	RegisterAssetTypeAction(MakeShareable(new FPlanetStaticEcoDataAssetTypeActions));
	*/
}

void FAndroidFaceCaptureEditor::ShutdownModule()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(StyleSet->GetStyleSetName());

	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for (auto CreatedAssetTypeAction : CreatedAssetTypeActions)
		{
			AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeAction.ToSharedRef());
		}
	}
	CreatedAssetTypeActions.Empty();
}

#undef LOCTEXT_NAMESPACE


