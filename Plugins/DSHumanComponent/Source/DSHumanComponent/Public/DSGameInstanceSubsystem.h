// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DSLibrary.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DSGameInstanceSubsystem.generated.h"


typedef void (*CheckAppCBK)(int*);
//DLL
typedef float* (*_update)();
typedef int(*_mainstart)();

typedef void (*_Init)(char* appID, char* appSecret, char* ip_, int port, CheckAppCBK cbk);

typedef void (*_DSInitBody)(float* arrPrt, float* QuaPrt, float* BodyPrt, int num, int dataOptions, float floatpy, int32 modelIndex, int32 personindex, int32 Origin);
typedef void (*_DSGetBodyResult)(int32 modelIndex, int32 personindex, float* hasBodyData, float* bodyRoate, float* bodyPos, float* rootResult);
typedef void (*_DSInitHand)(float* HandPrt, float* HandQua, float* HandPrtr, float* HandQuar, float* plam, int handType, int32 modelIndex, int32 personindex);
typedef void (*_DSGetHandResult)(int32 modelIndex, int32 personindex, float* lhandValid, float* lhandRoate, float* lhandPos, float* rhandValid, float* rhandRoate, float* rhandPos);

typedef void (*_SetRate)(float rate, float rate2, float rate3, float rate4, float rate5);
typedef void (*_DelMotion)(int32 modelIndex, int32 personindex);
typedef bool (*_DSCloseServer)();
typedef void (*_DSCleanMotion)();


static _Init  DLLInitEx = NULL;
static _DSInitBody DLLInitDSEx = NULL;
static _DSGetBodyResult DLLDSGetBodyResult = NULL;
static _DSInitHand DLLInitHandEx = NULL;
static _DSGetHandResult DLLDSGetHandResult = NULL;
static _SetRate DLLSetRateEx = NULL;
static _DelMotion DLLDelMotion = NULL;
static _DSCloseServer DLLCloseServer = NULL;
static _DSCleanMotion DLLCleanMotion = NULL;


/**
 *
 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FNetCheckDSEvent, bool, success);
struct FMocapDatas
{
	TArray<float> DLLBodyTickPre;
	TArray<float> DLLRightHandTickPre;
	TArray<float> DLLLeftHandTickPre;
	TArray<FVector> LocArray;

	TArray<FVector> LeftArray;
	TArray<FVector> RightArray;

	float fBetweenUpdate = 1;
	bool bGRootInit = true;
	bool bGHasHuman = true;
};

UCLASS()
class DSHUMANCOMPONENT_API UDSGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:

	TSharedPtr<FSocket> MySocket = nullptr;
	TSharedPtr<FInternetAddr> RemoteAddr;
	FString FuncNameStringBuffer;

	TArray<bool> bVisiabllist;

	TMap<int32, int8> DLLMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DSC")
		FString IP;

	FString LastIP = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DSC")
		int32 Port = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DSC|Dev")
		bool bSocketConnect = false;

	TArray< FMocapDatas> aAlldata;

	bool bHasInit = false;
	static bool hasInited;

	void* BodyDLLHandle = nullptr;

private:
	FTSTicker::FDelegateHandle CoreTickerHandle;
	FCriticalSection lDataLock;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection);

	virtual void Deinitialize() override;

	void CustomTick(float delta);

	void CustomBeginPlay();

	float fLocalTimer = 0.0f;

	FNetCheckDSEvent cbkConnection;

	UFUNCTION(BlueprintCallable, Category = "DSC|Socket")
		bool CreateSocket(FString IpString = TEXT("192.168.123.2"), int32 PortNumer = 6666);

	UFUNCTION(BlueprintCallable, Category = "DSC|Socket")
		bool ChangeSocket(FString IpString = TEXT("192.168.123.2"), int32 PortNumer = 6666);

	bool StartKcp(FString IpString = TEXT("192.168.123.2"), int32 PortNumer = 6666);


	UFUNCTION(BlueprintCallable, Category = "DSC|Function")
		void ClearSocket();

	void CloseKCP();

	bool GetVisible(int Index);

	bool SetbRootInit(int Index, bool CurBool);

	void CheckConnection();

	UFUNCTION(BlueprintCallable, Category = "DSC|Json")
		void BindConnectionCallback(const FNetCheckDSEvent& cbk);

	UFUNCTION()
		void GetDataArrFieldEx(int personIndex, TArray<FVector>& LocArray, TArray<FVector>& RightArray, TArray<FVector>& LeftArray,
			TArray<float>& CurDLLBodyTickPre, TArray<float>& CurDLLLeftHandTickPre, TArray<float>& CurDLLRightHandTickPre,
			bool& CurVisible, bool& CurbRootInit, bool& CurbHasHuman, int CurIndex);

	/*设置当前索引和当前索引的存在数量*/
	void SetDLLDataIndexMap(int32 CurIndex);

	/*清理当前索引的存在数量*/
	bool ClearDLLDataIndexMap(int32 CurIndex);

	//2023年10月8日13:58:14 @LF
	/*DLL_初始化*/
	void DLL_Begine();

};
