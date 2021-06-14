// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TcpSocketConnection.h"
#include "TCP_Data_Receiver.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTcpVectorDataDelegate, const TArray<FVector>&, VectorData, const FTransform&, CenterFaceT);

/**
 * 
 */
UCLASS()
class ANDROIDFACECAPTURE_API ATCP_Data_Receiver : public ATcpSocketConnection
{
	GENERATED_BODY()


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		FString IPAdress;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		int Port;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		bool EnableHeadTransformData;

	UPROPERTY(BlueprintReadOnly, Category = "Default")
		int Connect_ID;

	UPROPERTY(Transient)
		int bytesOfDataPacket = 0;
	UPROPERTY(Transient)
		int bytesOfHeaderPacket = 5;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Data")
		void SetVectorData(TArray<FVector>& Data);

	UFUNCTION()
		void DisconnectionD(int32 ID);
	UFUNCTION()
		void ConnectionD(int32 ID);

	void Message_ReadVectorArray(TArray<uint8>& Message, TArray<FVector>& Out, FMatrix& CenterFaceMat);
	UFUNCTION()
		void MessageReceptionD(int32 ID, TArray<uint8>& Msg);

	UPROPERTY(BlueprintAssignable)
		FTcpVectorDataDelegate SetData;

private:

	float AcuTime = 0.f;
	int receivedcount = 0;
	int receivedLatentcount = 0;

	bool Connected = false;
	float ReconnectTryAcu = 0.f;

	FTcpSocketDisconnectDelegate DisconnectedDelegate;
	FTcpSocketConnectDelegate ConnectedDelegate;
	FTcpSocketReceivedMessageDelegate MessageReceivedDelegate;

	TArray<uint8> LatentCopy;
	bool LatentSet = false;


	
};
