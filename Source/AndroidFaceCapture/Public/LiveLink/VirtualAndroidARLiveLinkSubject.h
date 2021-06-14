// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LiveLinkVirtualSubject.h"
#include "ILiveLinkClient.h"
#include "HAL/ThreadSafeBool.h"
#include "ARTrackable.h"
#include "Roles/LiveLinkAnimationTypes.h"
#include "LiveLinkAnimationVirtualSubject.h"
#include "VirtualSubjects/LiveLinkBlueprintVirtualSubject.h"
#include "VirtualAndroidARLiveLinkSubject.generated.h"

class AEditorTickingActors;
class UAndroidBlenshapeWeights;
/**
 * 
 */
UCLASS(Blueprintable, meta = (DisplayName = "Android FaceData Virtual Subject"))
class ANDROIDFACECAPTURE_API UVirtualAndroidARLiveLinkSubject : public ULiveLinkVirtualSubject
{
	GENERATED_BODY()


public:
	UVirtualAndroidARLiveLinkSubject();
	
	virtual void Update() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		UAndroidBlenshapeWeights* DataSource;

	FARBlendShapeMap FaceBlendShapes;
	
	// Builds a new ref skeleton based on the current subject state (can early out if ref skeleton is already up to date)
	void BuildSkeleton(const TArray<FLiveLinkSubjectFrameData>& InSubjectSnapshots);

	void BuildFrame(const TArray<FLiveLinkSubjectFrameData>& InSubjectSnapshots);

	// Tests to see if current ref skeleton is up to data
	bool DoesSkeletonNeedRebuilding() const;
	bool bInvalidate;
	

protected:

	bool BuildSubjectSnapshot(TArray<FLiveLinkSubjectFrameData>& OutSnapshot);
	
public:
	/*
	//Whether to append SubjectName to each bones part of the virtual hierarchy
	UPROPERTY(EditAnywhere, Category = "Settings")
		bool bAppendSubjectNameToBones;
	*/
};
