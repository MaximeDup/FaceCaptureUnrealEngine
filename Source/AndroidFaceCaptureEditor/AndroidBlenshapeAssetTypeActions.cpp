
#include "AndroidBlenshapeAssetTypeActions.h"
#include "AssetTypeCategories.h"
#include "Engine/EngineTypes.h"
#include "Modules/ModuleManager.h"
#include "AndroidBlenshape.h"
#include "Internationalization/Internationalization.h"

#define LOCTEXT_NAMESPACE "AndroidBlenshape"


FText FAndroidBlenshapeAssetTypeActions::GetName() const
{
	return LOCTEXT("FAndroidBlenshapeAssetTypeActionsName", "AndroidBlenshape");
}

FColor FAndroidBlenshapeAssetTypeActions::GetTypeColor() const
{
	return FColor::Cyan;
}

UClass* FAndroidBlenshapeAssetTypeActions::GetSupportedClass() const
{
	return UAndroidBlenshape::StaticClass();
}

uint32 FAndroidBlenshapeAssetTypeActions::GetCategories()
{
	return EAssetTypeCategories::Misc | MyAssetCategory;
}

#undef LOCTEXT_NAMESPACE

#define LOCTEXT_NAMESPACE "AndroidBlenshapeWeights"


FText FAndroidBlenshapeWeightsAssetTypeActions::GetName() const
{
	return LOCTEXT("FAndroidBlenshapeWeightsAssetTypeActionsName", "AndroidBlenshapeWeights");
}

FColor FAndroidBlenshapeWeightsAssetTypeActions::GetTypeColor() const
{
	return FColor::Cyan;
}

UClass* FAndroidBlenshapeWeightsAssetTypeActions::GetSupportedClass() const
{
	return UAndroidBlenshapeWeights::StaticClass();
}

uint32 FAndroidBlenshapeWeightsAssetTypeActions::GetCategories()
{
	return EAssetTypeCategories::Misc | MyAssetCategory;
}

#undef LOCTEXT_NAMESPACE
/*
#define LOCTEXT_NAMESPACE "PlanetSpawnableMesh"


FText FPlanetSpawnableMeshAssetTypeActions::GetName() const
{
	return LOCTEXT("FPlanetSpawnableMeshAssetTypeActionsName", "PlanetSpawnableMesh");
}

FColor FPlanetSpawnableMeshAssetTypeActions::GetTypeColor() const
{
	return FColor::Cyan;
}

UClass* FPlanetSpawnableMeshAssetTypeActions::GetSupportedClass() const
{
	return UPlanetSpawnableMesh::StaticClass();
}

uint32 FPlanetSpawnableMeshAssetTypeActions::GetCategories()
{
	return EAssetTypeCategories::Misc | MyAssetCategory;
}

#undef LOCTEXT_NAMESPACE

#define LOCTEXT_NAMESPACE "PlanetGroundMaterial"


FText FPlanetGroundMaterialAssetTypeActions::GetName() const
{
	return LOCTEXT("FPlanetGroundMaterialAssetTypeActionsName", "PlanetGroundMaterial");
}

FColor FPlanetGroundMaterialAssetTypeActions::GetTypeColor() const
{
	return FColor::Cyan;
}

UClass* FPlanetGroundMaterialAssetTypeActions::GetSupportedClass() const
{
	return UPlanetGroundMaterial::StaticClass();
}

uint32 FPlanetGroundMaterialAssetTypeActions::GetCategories()
{
	return EAssetTypeCategories::Misc | MyAssetCategory;
}

#undef LOCTEXT_NAMESPACE




#define LOCTEXT_NAMESPACE "PlanetStaticEcoData"


FText FPlanetStaticEcoDataAssetTypeActions::GetName() const
{
	return LOCTEXT("FPlanetStaticEcoDataAssetTypeActionsName", "PlanetStaticEcoData");
}

FColor FPlanetStaticEcoDataAssetTypeActions::GetTypeColor() const
{
	return FColor::Cyan;
}

UClass* FPlanetStaticEcoDataAssetTypeActions::GetSupportedClass() const
{
	return UPlanetStaticEcoData::StaticClass();
}

uint32 FPlanetStaticEcoDataAssetTypeActions::GetCategories()
{
	return EAssetTypeCategories::Misc | MyAssetCategory;
}

#undef LOCTEXT_NAMESPACE
*/