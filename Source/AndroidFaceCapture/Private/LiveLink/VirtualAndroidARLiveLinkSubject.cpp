// Fill out your copyright notice in the Description page of Project Settings.


#include "LiveLink/VirtualAndroidARLiveLinkSubject.h"
#include "Roles/LiveLinkBasicRole.h"
#include "EditorTickingActors.h"
#include "EngineUtils.h" // for TActorIterator
#include "Roles/LiveLinkAnimationRole.h"
#include "Roles/LiveLinkAnimationTypes.h"



namespace LiveLinkAndAnimationVirtualSubjectUtils
{
	void AddToBoneNames(TArray<FName>& BoneNames, const TArray<FName>& NewBoneNames, const FName Prefix)
	{
		FString NameFormat;
		if (Prefix != NAME_None)
		{
			NameFormat = Prefix.ToString() + TEXT("_");
		}

		BoneNames.Reserve(BoneNames.Num() + NewBoneNames.Num());

		for (const FName& NewBoneName : NewBoneNames)
		{
			BoneNames.Add(*(NameFormat + NewBoneName.ToString()));
		}
	}

	void AddToBoneParents(TArray<int32>& BoneParents, const TArray<int32>& NewBoneParents)
	{
		const int32 Offset = BoneParents.Num();

		BoneParents.Reserve(BoneParents.Num() + NewBoneParents.Num());

		for (int32 BoneParent : NewBoneParents)
		{
			// Here we are combining multiple bone hierarchies under one root bone
			// Each hierarchy is complete self contained so we have a simple calculation to perform
			// 1) Bones with out a parent get parented to root (-1 => 0 )
			// 2) Bones with a parent need and offset based on the current size of the buffer
			if (BoneParent == INDEX_NONE)
			{
				BoneParents.Add(0);
			}
			else
			{
				BoneParents.Add(BoneParent + Offset);
			}
		}
	}
}

UVirtualAndroidARLiveLinkSubject::UVirtualAndroidARLiveLinkSubject()
{
	Role = ULiveLinkBasicRole::StaticClass();

}

void UVirtualAndroidARLiveLinkSubject::Update()
{
	Super::Update();

	TArray<FLiveLinkSubjectKey> ActiveSubjects;
	ActiveSubjects.Add(SubjectKey);

	if (!DataSource)
		return;

	if (DataSource && !DataSource->WeightsRead)
	{		
		FaceBlendShapes = DataSource->BlendShapeMap;
		DataSource->WeightsRead = true;
	}

	const UEnum* EnumPtr = StaticEnum<EARFaceBlendShape>();
	if (EnumPtr == nullptr)
	{
		return;
	}

	TArray<FLiveLinkSubjectFrameData> SubjectSnapshot;
	if (BuildSubjectSnapshot(SubjectSnapshot))
	{

		BuildSkeleton(SubjectSnapshot);
		BuildFrame(SubjectSnapshot);
	}	

}



void UVirtualAndroidARLiveLinkSubject::BuildSkeleton(const TArray<FLiveLinkSubjectFrameData>& InSubjectSnapshots)
{


	if (DoesSkeletonNeedRebuilding())
	{
		FrameSnapshot.StaticData.InitializeWith(FLiveLinkSkeletonStaticData::StaticStruct(), nullptr);
		FLiveLinkSkeletonStaticData* SkeletonData = FrameSnapshot.StaticData.Cast<FLiveLinkSkeletonStaticData>();
		/*
		TArray<FName> BoneNames{ TEXT("Root") };
		TArray<int32> BoneParents{ INDEX_NONE };

		check(InSubjectSnapshots.Num() == Subjects.Num());
		for (int32 i = 0; i < InSubjectSnapshots.Num(); ++i)
		{
			const FLiveLinkSubjectFrameData& SubjectSnapShotData = InSubjectSnapshots[i];
			check(SubjectSnapShotData.StaticData.IsValid());
			const FLiveLinkSkeletonStaticData* SubjectSkeletonData = SubjectSnapShotData.StaticData.Cast<FLiveLinkSkeletonStaticData>();

			const FName BonePrefix = bAppendSubjectNameToBones ? Subjects[i] : NAME_None;
			LiveLinkAndAnimationVirtualSubjectUtils::AddToBoneNames(BoneNames, SubjectSkeletonData->GetBoneNames(), BonePrefix);
			LiveLinkAndAnimationVirtualSubjectUtils::AddToBoneParents(BoneParents, SubjectSkeletonData->GetBoneParents());
			SkeletonData->PropertyNames.Append(SubjectSkeletonData->PropertyNames);
		}

		SkeletonData->SetBoneNames(BoneNames);
		SkeletonData->SetBoneParents(BoneParents);
		*/
		bInvalidate = false;
	}

}

static FName ParseEnumName(FName EnumName)
{
	const int32 BlendShapeEnumNameLength = 19;
	FString EnumString = EnumName.ToString();
	return FName(*EnumString.Right(EnumString.Len() - BlendShapeEnumNameLength));
}

void UVirtualAndroidARLiveLinkSubject::BuildFrame(const TArray<FLiveLinkSubjectFrameData>& InSubjectSnapshots)
{
	

	if (!FrameSnapshot.FrameData.IsValid())
	{
		FrameSnapshot.FrameData.InitializeWith(FLiveLinkAnimationFrameData::StaticStruct(), nullptr);
	}

	FLiveLinkSkeletonStaticData* SnapshotSkeletonData = FrameSnapshot.StaticData.Cast<FLiveLinkSkeletonStaticData>();
	FLiveLinkAnimationFrameData* SnapshotFrameData = FrameSnapshot.FrameData.Cast<FLiveLinkAnimationFrameData>();


	//Update property names array
	SnapshotSkeletonData->PropertyNames.Reset((int32)EARFaceBlendShape::MAX);

	//BlendShapeData.StaticData.PropertyNames.Reset((int32)EARFaceBlendShape::MAX);

	//Iterate through all valid blend shapes to extract names
	const UEnum* EnumPtr = StaticEnum<EARFaceBlendShape>();
	for (int32 Shape = 0; Shape < (int32)EARFaceBlendShape::MAX; Shape++)
	{
		if (FaceBlendShapes.Contains((EARFaceBlendShape)Shape))
		{
			//Blendshapes don't change over time. If they were to change, we would need to keep track
			//of previous values to always have frame data matching static data
			const FName ShapeName = ParseEnumName(EnumPtr->GetNameByValue(Shape));
			SnapshotSkeletonData->PropertyNames.Add(ShapeName);
		}
	}

	// Iterate through all of the blend shapes copying them into the LiveLink data type
	for (int32 Shape = 0; Shape < (int32)EARFaceBlendShape::MAX; Shape++)
	{
		if (FaceBlendShapes.Contains((EARFaceBlendShape)Shape))
		{
			const float CurveValue = FaceBlendShapes.FindChecked((EARFaceBlendShape)Shape);
			SnapshotFrameData->PropertyValues.Add(CurveValue);

		}
	}



	SnapshotFrameData->Transforms.Reset(SnapshotSkeletonData->GetBoneNames().Num());
	SnapshotFrameData->Transforms.Add(FTransform::Identity);
	SnapshotFrameData->MetaData.StringMetaData.Empty();

	//Go over each subject snapshot and take transforms and curves
	check(InSubjectSnapshots.Num() == Subjects.Num());
	for (int32 i = 0; i < InSubjectSnapshots.Num(); ++i)
	{
		const FLiveLinkSubjectFrameData& SubjectSnapShotData = InSubjectSnapshots[i];
		check(SubjectSnapShotData.FrameData.IsValid());
		const FLiveLinkAnimationFrameData* SubjectFrameData = SubjectSnapShotData.FrameData.Cast<FLiveLinkAnimationFrameData>();

		SnapshotFrameData->Transforms.Append(SubjectFrameData->Transforms);
		SnapshotFrameData->PropertyValues.Append(SubjectFrameData->PropertyValues);
		for (const auto& MetaDatum : SubjectFrameData->MetaData.StringMetaData)
		{
			const FName QualifiedKey = FName(*(Subjects[i].ToString() + MetaDatum.Key.ToString()));
			SnapshotFrameData->MetaData.StringMetaData.Emplace(Subjects[i], MetaDatum.Value);
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("BuildFrame"));
}

bool UVirtualAndroidARLiveLinkSubject::DoesSkeletonNeedRebuilding() const
{	
	return !FrameSnapshot.StaticData.IsValid() || bInvalidate;	
}

bool UVirtualAndroidARLiveLinkSubject::BuildSubjectSnapshot(TArray<FLiveLinkSubjectFrameData>& OutSnapshot)
{
	//UE_LOG(LogTemp, Warning, TEXT("BuildSubjectSnapshot Subjects.Num() %d"), Subjects.Num());

	OutSnapshot.Reset(Subjects.Num());

	bool bSnapshotDone = true;

	for (const FName& SubjectName : Subjects)
	{
		FLiveLinkSubjectFrameData& NextSnapshot = OutSnapshot.AddDefaulted_GetRef();
		if (!LiveLinkClient->EvaluateFrame_AnyThread(SubjectName, GetRole(), NextSnapshot))
		{
			bSnapshotDone = false;
			break;
		}
	}

	return bSnapshotDone;
}