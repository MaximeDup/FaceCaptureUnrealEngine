// Fill out your copyright notice in the Description page of Project Settings.


#include "TCP_Data_Receiver.h"
#include "Kismet/KismetStringLibrary.h"
#include "Async/ParallelFor.h"

void ATCP_Data_Receiver::BeginPlay()
{
	Super::BeginPlay();

	/*
		FTcpSocketDisconnectDelegate DisconnectedDelegate;
	FTcpSocketConnectDelegate ConnectedDelegate;
	FTcpSocketReceivedMessageDelegate MessageReceivedDelegate;
	*/
	DisconnectedDelegate.BindDynamic(this, &ATCP_Data_Receiver::DisconnectionD);  //.BindDynamic(&DisconnectionD);
	ConnectedDelegate.BindDynamic(this, &ATCP_Data_Receiver::ConnectionD);  // .BindRaw(&ConnectionD);
	MessageReceivedDelegate.BindDynamic(this, &ATCP_Data_Receiver::MessageReceptionD);

	Connect(IPAdress, Port, DisconnectedDelegate, ConnectedDelegate, MessageReceivedDelegate, Connect_ID);
}

void ATCP_Data_Receiver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AcuTime += DeltaTime;

	if (AcuTime > 1.f)
	{
		AcuTime = 0.f;

		UE_LOG(LogTemp, Warning, TEXT("receivedcount %d receivedLatentcount %d"), receivedcount, receivedLatentcount);
		receivedcount = 0;
		receivedLatentcount = 0;
	}
	
	if (!Connected)
	{
		ReconnectTryAcu += DeltaTime;

		if (ReconnectTryAcu > 1.f)
		{
			ReconnectTryAcu = 0.f;
			Connect(IPAdress, Port, DisconnectedDelegate, ConnectedDelegate, MessageReceivedDelegate, Connect_ID);
		}
	}
	

}

void ATCP_Data_Receiver::DisconnectionD(int32 ID)
{
	Connected = false;
}

void ATCP_Data_Receiver::ConnectionD(int32 ID)
{
	Connected = true;
}

void ATCP_Data_Receiver::Message_ReadVectorArray(TArray<uint8>& Message, TArray<FVector>& Out, FMatrix& CenterFaceMat)
{

	int Offset = 0;
	while (Offset <= Message.Num() - 12 - 64)
	{
		float X, Y, Z;
		unsigned char byteArray[4];

		for (int i = 0; i < 4; i++)
		{
			byteArray[i] = Message[3 - i + Offset];
		}

		FMemory::Memcpy(&X, byteArray, 4);

		for (int i = 0; i < 4; i++)
		{
			byteArray[i] = Message[3 - i + 4 + Offset];
		}

		FMemory::Memcpy(&Y, byteArray, 4);

		for (int i = 0; i < 4; i++)
		{
			byteArray[i] = Message[3 - i + 8 + Offset];
		}

		FMemory::Memcpy(&Z, byteArray, 4);

		Out.Add(FVector(X, Y, Z));

		Offset = Offset + 12;
	}

	//The last 16 float describe a columnMajor matrix for the center of the face
	//UE_LOG(LogTemp, Warning, TEXT("remaining floats %d"), Message.Num() - Offset);

	TArray<float> MatrixFloatsColumnFirst;

	for (int i = 0; i < 16; i++)
	{
		float X;
		unsigned char byteArray[4];

		for (int j = 0; j < 4; j++)
		{
			byteArray[j] = Message[3 - j + Offset];
		}
		FMemory::Memcpy(&X, byteArray, 4);

		Offset = Offset + 4;

		MatrixFloatsColumnFirst.Add(X);
	}

	// M[row][Column] and MatrixFloatsColumnFirst is 16 floats Column first [C1_1][C1_2][C1_33]...
	CenterFaceMat = FMatrix::Identity;

	FString DebugMat = "";

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			int index = i * 4 + j;
			
			CenterFaceMat.M[j][i] = MatrixFloatsColumnFirst[index];

			//DebugMat += "M.[" + FString::SanitizeFloat(j) + "][" + FString::SanitizeFloat(i)+"] = " + FString::SanitizeFloat(MatrixFloatsColumnFirst[index])+ (j==3? "\n" :" ");
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("DebugMat = \n %s"), *DebugMat);

}

void ATCP_Data_Receiver::MessageReceptionD(int32 ID, TArray<uint8>& Msg)
{

	if (Msg.Num() == 0)
		return;

	FMatrix CenterFace;

	FTransform CenterFaceT;	
	CenterFaceT.SetLocation(FVector(0.f, 0.f, 0.f));
	CenterFaceT.SetScale3D(FVector(1.f, 1.f, 1.f));
	CenterFaceT.SetRotation(FQuat::Identity);
	

	if (Msg.Num() == 5680)
	{
		LatentSet = false;
		LatentCopy.Empty();

		TArray<FVector> Data_Vector;
		Message_ReadVectorArray(Msg, Data_Vector, CenterFace);

		if (EnableHeadTransformData)
		{
			CenterFaceT.SetLocation(CenterFace.TransformPosition(FVector(0.f, 0.f, 0.f)));
			CenterFaceT.SetRotation(CenterFace.Rotator().Quaternion());
		}		

		if (SetData.IsBound())
			SetData.Broadcast(Data_Vector, CenterFaceT);
		receivedcount++;

		return;
	}


	//UE_LOG(LogTemp, Warning, TEXT("Message length %d latent length %d"), Msg.Num(), LatentCopy.Num());

	if (!LatentSet)
	{
		while (Msg.Num() >= 5680)
		{
			LatentSet = false;
			LatentCopy.Empty();

			TArray<uint8> DummyTemp;
			//Message_ReadBytes(5616, Msg, DummyTemp);

			for (int i = 0; i < 5680; i++)
			{
				DummyTemp.Add(Message_ReadByte(Msg));
			}

			TArray<FVector> Data_Vector;
			Message_ReadVectorArray(DummyTemp, Data_Vector, CenterFace);

			if (EnableHeadTransformData)
			{
				CenterFaceT.SetLocation(CenterFace.TransformPosition(FVector(0.f, 0.f, 0.f)));
				CenterFaceT.SetRotation(CenterFace.Rotator().Quaternion());
			}
			

			if (SetData.IsBound())
				SetData.Broadcast(Data_Vector, CenterFaceT);

			receivedcount++;
		}
	}


	if (LatentSet)
	{
		if ((LatentCopy.Num() + Msg.Num()) % 5680 == 0)
		{
			LatentSet = false;
			LatentCopy.Append(Msg);

			while (LatentCopy.Num() >= 5680)
			{

				TArray<uint8> DummyTemp;
				//Message_ReadBytes(5616, Msg, DummyTemp);

				for (int i = 0; i < 5680; i++)
				{
					DummyTemp.Add(Message_ReadByte(LatentCopy));
				}

				TArray<FVector> Data_Vector;
				Message_ReadVectorArray(DummyTemp, Data_Vector, CenterFace);

				if (EnableHeadTransformData)
				{
					CenterFaceT.SetLocation(CenterFace.TransformPosition(FVector(0.f, 0.f, 0.f)));
					CenterFaceT.SetRotation(CenterFace.Rotator().Quaternion());
				}

				if (SetData.IsBound())
					SetData.Broadcast(Data_Vector, CenterFaceT);

				receivedcount++;
				receivedLatentcount++;
			}

			LatentCopy.Empty();

		}
	}
	else
	{
		LatentCopy = Msg;
		LatentSet = true;
	}

}

void ATCP_Data_Receiver::SetVectorData_Implementation(TArray<FVector>& Data)
{

}
