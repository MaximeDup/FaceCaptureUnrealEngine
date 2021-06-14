
#pragma once

#include "CoreMinimal.h"
#include "Toolkits/IToolkitHost.h"
#include "AssetTypeActions_Base.h"

class UAndroidBlenshape;

class FAndroidBlenshapeAssetTypeActions : public FAssetTypeActions_Base
{
public:
	//FEcoSystemAssetTypeActions(EAssetTypeCategories::Type InAssetCategory);

	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;

private:
	EAssetTypeCategories::Type MyAssetCategory;
};

class FAndroidBlenshapeWeightsAssetTypeActions : public FAssetTypeActions_Base
{
public:
	//FEcoSystemAssetTypeActions(EAssetTypeCategories::Type InAssetCategory);

	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;

private:
	EAssetTypeCategories::Type MyAssetCategory;
};
/*
class UEcoSystem;


class FEcoSystemAssetTypeActions : public FAssetTypeActions_Base
{
public:
	//FEcoSystemAssetTypeActions(EAssetTypeCategories::Type InAssetCategory);

	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;
	
private:
	EAssetTypeCategories::Type MyAssetCategory;
};

class FPlanetSpawnableMeshAssetTypeActions : public FAssetTypeActions_Base
{
public:
	//FEcoSystemAssetTypeActions(EAssetTypeCategories::Type InAssetCategory);

	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;

private:
	EAssetTypeCategories::Type MyAssetCategory;
};

class FPlanetGroundMaterialAssetTypeActions : public FAssetTypeActions_Base
{
public:
	//FEcoSystemAssetTypeActions(EAssetTypeCategories::Type InAssetCategory);

	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;

private:
	EAssetTypeCategories::Type MyAssetCategory;
};

class FPlanetStaticEcoDataAssetTypeActions : public FAssetTypeActions_Base
{
public:
	//FEcoSystemAssetTypeActions(EAssetTypeCategories::Type InAssetCategory);

	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;

private:
	EAssetTypeCategories::Type MyAssetCategory;
};*/