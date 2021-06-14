// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AndroidBlenshape.h"
#include "EditorTickingActors.generated.h"

class UAndroidBlenshapeWeights;
UCLASS()
class ANDROIDFACECAPTURE_API AEditorTickingActors : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEditorTickingActors();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ComputeBlendWeights();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



#if WITH_EDITOR

	bool ShouldTickIfViewportsOnly() const override;

#endif

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		bool SelectReferenceLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
		EARFaceBlendShape Selected_Location;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		TArray<float> Weights;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		float DistanceDependency=100.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		TArray<FVector> CurrentPose;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		FRotator HeadRotation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		FRotator WS_PawnRotation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		UAndroidBlenshape* ReferenceBladeShape;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Default")
		TArray<UAndroidBlenshape*>BlendShapes;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	UAndroidBlenshapeWeights* ToLiveLinkData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
		TMap<EARFaceBlendShape, float> BlendShapeMap;

private:

	float TimeAcuBlendW = 0.f;

};
