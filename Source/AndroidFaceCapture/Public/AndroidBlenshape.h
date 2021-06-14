// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HAL/ThreadSafeBool.h"
#include "ARTrackable.h"
#include "AndroidBlenshape.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ANDROIDFACECAPTURE_API UAndroidBlenshape : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
		bool LocationEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	EARFaceBlendShape Location_Shape;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
		bool UseAsReferencePose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default")
		FString Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
		float Value=0.f;

	float GetBlendShapeValue() { return Value; };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
		float DistanceInfluence = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
		float DistanceOffset = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
		TArray<int> VerticesToControl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
		TArray<FVector> VerticesMaxMvt;

};


/* Currently, data is received via TCP in an Actor set in the world (free TCP socket plugin), this actor convert the incoming byte array to the valuable vertices position and head transform.
*	Another actor process this information to compute the blendweights by comparing incoming data to predefined blendshape references (another tool/actor is in the world to define a subset of vertices relevant for a given blenshape)
* BlendWeights are stored in a UAndroidBlenshapeWeights and WeightsRead is set to false
* Once the livelink subject is updated, it read the blendweights from UAndroidBlenshapeWeights, unlocked it by setting WeightsRead to true, informs the blenshapes in a similar fashion than ARKit does
	Alternative would be to convert the TCP plugin to work with UObject instead of actors and do everything as a livelink source subject, including the blend weight computation. Currently i'm computing the blend weights at runtime in an Actor.
	Or disregarding Livelink alltogether and figuring out how to send the blendshapes to the animBP directly.
	*/
UCLASS(/*Blueprintable, BlueprintType*/ )
class ANDROIDFACECAPTURE_API UAndroidBlenshapeWeights : public UObject
{
	GENERATED_BODY()

public:

	/* Weights are ordered as in EARFaceBlendShape*/
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TArray<float> BlendWeights;

	FARBlendShapeMap BlendShapeMap;

	FThreadSafeBool WeightsRead;

};
