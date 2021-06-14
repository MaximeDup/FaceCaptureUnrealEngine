// Fill out your copyright notice in the Description page of Project Settings.


#include "AndroidBlenshapeFactory.h"
#include "AndroidFaceCaptureEditor.h"
//The asset header file that we wish to create
//#include "UEcoSystem.h"
//The asset type categories will let us access the various asset categories inside the Editor
#include "AssetTypeCategories.h"

#define LOCTEXT_NAMESPACE "AndroidBlenshapeFactory"

UAndroidBlendFactory::UAndroidBlendFactory(const FObjectInitializer & ObjectInitializer)
		: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UAndroidBlenshape::StaticClass();
}


uint32 UAndroidBlendFactory::GetMenuCategories() const
{
	//Let's place this asset in the Blueprints category in the Editor
	return FAndroidFaceCaptureEditor::GetAssetCategory();
}

UObject* UAndroidBlendFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	//Create the editor asset 
	UAndroidBlenshape* OrfeasEditorAsset = NewObject<UAndroidBlenshape>(InParent, InClass, InName, Flags);
	return OrfeasEditorAsset;
}

#undef LOCTEXT_NAMESPACE



#define LOCTEXT_NAMESPACE "AndroidBlenshapeWeightsFactory"

UAndroidBlenshapeWeightsFactory::UAndroidBlenshapeWeightsFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UAndroidBlenshapeWeights::StaticClass();
}


uint32 UAndroidBlenshapeWeightsFactory::GetMenuCategories() const
{
	//Let's place this asset in the Blueprints category in the Editor
	return FAndroidFaceCaptureEditor::GetAssetCategory();
}

UObject* UAndroidBlenshapeWeightsFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	//Create the editor asset 
	UAndroidBlenshapeWeights* OrfeasEditorAsset = NewObject<UAndroidBlenshapeWeights>(InParent, InClass, InName, Flags);
	return OrfeasEditorAsset;
}

#undef LOCTEXT_NAMESPACE
