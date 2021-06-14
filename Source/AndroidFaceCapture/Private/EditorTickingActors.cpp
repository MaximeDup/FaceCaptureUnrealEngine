// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorTickingActors.h"
#include "Async/ParallelFor.h"


// Sets default values
AEditorTickingActors::AEditorTickingActors()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEditorTickingActors::BeginPlay()
{
	Super::BeginPlay();

	if (ToLiveLinkData && !ToLiveLinkData->WeightsRead)
		ToLiveLinkData->WeightsRead = true;
	
}

void AEditorTickingActors::ComputeBlendWeights()
{

	ParallelFor(BlendShapes.Num(), [&](int32 Index) {

		// For now just computing the average distance between the reference pose and the blenshape pose scaled by DistanceInfluence
		if (BlendShapes[Index]->UseAsReferencePose)
		{
			Weights[Index] = 1.f;
			return;
		}
		if (!BlendShapes[Index]->LocationEnabled)
		{
			Weights[Index] = 0.f;
			BlendShapes[Index]->Value = 0.f;
			return;
		}
			
		//For a given BlendShape, get the indices of vertices relevant to monitor
		TArray<int>& vertices = BlendShapes[Index]->VerticesToControl;

		//For a given BlendShape, get s user define scalar to scale the 
		float BlendShapeCustomDistanceInfluence = BlendShapes[Index]->DistanceInfluence;

		int NumOfvertices = vertices.Num();
		if (NumOfvertices > 0)
		{
			float Distance = 0.f;

			for (int i = 0; i < NumOfvertices; i++)
			{
				//Distance += FMath::Clamp(1.0f - BlendShapeCustomDistanceInfluence * DistanceDependency * (CurrentPose[vertices[i]] - BlendShapes[Index]->VerticesMaxMvt[vertices[i]]).Size() - BlendShapes[Index]->DistanceOffset, 0.f, 1.f);

				FVector Curent_minus_Blendshape = CurrentPose[vertices[i]] - BlendShapes[Index]->VerticesMaxMvt[vertices[i]];

				FVector Reference_minus_Blendshape = ReferenceBladeShape->VerticesMaxMvt[vertices[i]]- BlendShapes[Index]->VerticesMaxMvt[vertices[i]];

				float DotP = FMath::Clamp(DistanceDependency*(FVector::DotProduct(Curent_minus_Blendshape, Reference_minus_Blendshape) - BlendShapes[Index]->DistanceOffset*0.001f),0.f,1.f);

				Distance += 1.0f - DotP;

				//Distance += FMath::Clamp(1.0f - BlendShapeCustomDistanceInfluence * DistanceDependency * (CurrentPose[vertices[i]] - BlendShapes[Index]->VerticesMaxMvt[vertices[i]]).Size() - BlendShapes[Index]->DistanceOffset, 0.f, 1.f);
			}

			float weight = Distance /= NumOfvertices;
			Weights[Index] = weight;
			BlendShapes[Index]->Value = weight;
		}


	});

}

// Called every frame
void AEditorTickingActors::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeAcuBlendW += DeltaTime;
	if (TimeAcuBlendW > 1.f / 30.f)
	{
		TimeAcuBlendW = 0.f;

		if (ToLiveLinkData && ToLiveLinkData->WeightsRead)
		{		
		
			if (CurrentPose.Num() == 468)
			{
				if (ReferenceBladeShape)
				{

					Weights.SetNum(BlendShapes.Num());

					ComputeBlendWeights();


					ToLiveLinkData->BlendShapeMap.Empty();

					for (int i = 0; i < Weights.Num(); i++)
					{
						if(!BlendShapes[i]->UseAsReferencePose)
							ToLiveLinkData->BlendShapeMap.Add(BlendShapes[i]->Location_Shape, BlendShapes[i]->Value);
					}
					

					FRotator Head_R_LS = (HeadRotation.Quaternion()*FRotator(0.f,90.f,0.f).Quaternion() * WS_PawnRotation.Quaternion().Inverse()).Inverse().Rotator();

				

					ToLiveLinkData->BlendShapeMap.Add(EARFaceBlendShape::HeadYaw, Head_R_LS.Yaw/180.f*3.14156535f);
					ToLiveLinkData->BlendShapeMap.Add(EARFaceBlendShape::HeadRoll, HeadRotation.Pitch  / 180.f * 3.14156535f);
					ToLiveLinkData->BlendShapeMap.Add(EARFaceBlendShape::HeadPitch, -HeadRotation.Roll / 180.f * 3.14156535f);

					BlendShapeMap = ToLiveLinkData->BlendShapeMap;

					ToLiveLinkData->BlendWeights = Weights;

					ToLiveLinkData->WeightsRead = false;

				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Reference blenshape Pose not set"));
				}

			}
		}
	}


}

#if WITH_EDITOR
bool AEditorTickingActors::ShouldTickIfViewportsOnly() const
{
	return true;
}
#endif
