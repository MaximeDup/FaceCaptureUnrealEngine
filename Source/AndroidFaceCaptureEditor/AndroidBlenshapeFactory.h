// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

#include "Factories/Factory.h"
#include "AndroidBlenshape.h"
#include "AndroidBlenshapeFactory.generated.h"




UCLASS()
class ANDROIDFACECAPTUREEDITOR_API UAndroidBlendFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

public:

	
	virtual uint32 GetMenuCategories() const override;


	
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

	
};


UCLASS()
class ANDROIDFACECAPTUREEDITOR_API UAndroidBlenshapeWeightsFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

public:


	virtual uint32 GetMenuCategories() const override;



	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;


};


