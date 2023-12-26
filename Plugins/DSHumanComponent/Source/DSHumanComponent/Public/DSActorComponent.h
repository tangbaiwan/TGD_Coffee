// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DSLibrary.h"
#include "Components/ActorComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Serialization/JsonSerializer.h"
#include "Runtime/Sockets/Public/Sockets.h"
#include "Runtime/Networking/Public/Networking.h"

#include "DSGameInstanceSubsystem.h"
#include "DSUsersSettings.h"
#include "DSActorComponent.generated.h"





UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DSHUMANCOMPONENT_API UDSActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDSActorComponent();

	UDSGameInstanceSubsystem* DSISubsystem = nullptr;

	UDSUsersSettings* DSUsersSet = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DSC", meta = (DisplayName = "采集方式"))
		EDataOptions DataOptions = EDataOptions::monocular;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DSC", meta = (DisplayName = "面捕方案"))
		ELinkOptions FaceLinkOptions;


	/*手动设置移动旋转位置*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DSC", meta = (DisplayName = "位置偏移"))
		FTransform Offest;

	//记录上一帧旋转数值
	TArray<FVector> LastRootLocArray;

	///*没有采集数据时是否隐藏*/
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DSC", meta = (DisplayName = "没有采集数据时是否隐藏"))
	bool bHiddenByDataNull = true;

	/*使用采集端识别到的第几位角色（非多人模式默认为0）*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DSC", meta = (DisplayName = "使用采集端识别到的第几位角色"))
		int iPersonIndex = 0;

	///*重置当前偏移位置为起始点位置*/
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DSC", meta = (DisplayName = "重置位置"))
	//	bool bReSetRootLocOffest = false;

	FVector CurbReSetRootLocOffest;

	/*取消勾选将使用相机位置作为原点*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DSC|Dev", meta = (DisplayName = "使用Actor位置作为原点"))
		bool bOrigin = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DSC|Dev", meta = (DisplayName = "打印日志"))
		bool bPrintLog = false;


	/*取消勾选将固定下半身*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DSC|Dev", meta = (DisplayName = "是否驱动下半身"))
		bool bChangeLowerBody = true;


	/*骨骼模型的匹配方案索引，默认为0或-1.将使用MetaHuman标准骨骼*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DSC|Dev", meta = (DisplayName = "骨骼模型的匹配方案索引"))
		int SelectedOptions = -1;

	/*脚步偏移系数，改善滑步，系数越大，角色移动越明显*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DSC|Dev", meta = (DisplayName = "脚步偏移系数"))
		float FootOffestCtrl = 1.0f;

	/*脚步贴地功能*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DSC|Dev", meta = (DisplayName = "是否开启脚步贴地"))
		bool bFootTracFloor = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DSC|Dev", meta = (EditCondition = "bFootTracFloor", DisplayName = "脚步贴地后偏移的高度"))
		float FootDistance = 3.0f;
	///*第几帧开始接收采集数据，默认为第5帧*/
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DSC|Dev", meta = (DisplayName = "第几帧开始接收采集数据", ClampMin = "1", ClampMax = "30"))
	uint8 TickNumber = 5;


	/*程序运行后可以查看当前APPID是否生效*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DSC|RunTime", meta = (DisplayName = "APPID是否生效"))
		bool ActorDLLInited = false;

	//程序运行后可以查看当前初始化数据索引
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DSC|RunTime", meta = (DisplayName = "当前DLL唯一索引"))
		int32 DLLDataIndex = -1;

	///*查看当前移动的位置*/
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DSC|RunTime", meta = (DisplayName = "当前移动的位置"))
	//	FVector CurPelvisLoc;

	float MaxFloat, MinFloat, FigMaxFloat, FigMinFloat, floatpy;

	UPROPERTY(BlueprintReadWrite, Category = "DSC")
		USkeletalMeshComponent* CurrentSkeMesh;


	FVector offsetRootEx;

	//身体pelvis位置
	UPROPERTY(BlueprintReadWrite, Category = "DSC")
		FVector pelvisSlotLocationByTick = FVector::ZeroVector;

	//每一帧清空当前位置数据
	UPROPERTY(BlueprintReadWrite, Category = "DSC")
		TArray<FVector> LocArrayTick;

	TArray<FVector> RightArrayTick;
	TArray<FVector> LeftArrayTick;

	TArray<float> FaceDataTick;

	TArray<FName> BodySocketName;
	TArray<FName> LeftSocketName;
	TArray<FName> RightSocketName;

	//DLL
	TArray<float> DLLRightHandTickPre;
	TArray<float> DLLLeftHandTickPre;

	TArray<float> DLLBodyTickPre;

	bool bRootInit = true;
	bool bHasHuman = true;

	float traceHeight = 150.0f;
	float preHeight = 0.0f;

	uint8 RootInitIndex = 2;

	float LastTickHand_l_Value;
	float LastTickHand_r_Value;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FRotator> DLLBodyRotArray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FRotator> DLLLeftHandRotArray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FRotator> DLLRightHandRotArray;

	//保存当前四元数
	TArray<FQuat> DLLLeftHandQuat, DLLRightHandQuat;

	//面部数据
	UPROPERTY(BlueprintReadWrite, Category = "DSC|Struct")
		TMap<FName, float> FaceFrame;

	//面部权重名字
	//@李洋啸
	UPROPERTY(BlueprintReadWrite, Category = "DSC|Struct")
		TArray<FName> FaceNames = { "browDownLeft",
		"browDownRight",
		"browInnerUp",
		"browInnerUp",
		"browOuterUpLeft",
		"browOuterUpRight",
		"cheekPuff",
		"cheekPuff",
		"cheekSquintLeft",
		"cheekSquintRight",
		"eyeBlinkLeft",
		"eyeBlinkRight",
		"eyeLookDownLeft",
		"eyeLookDownRight",
		"eyeLookInLeft",
		"eyeLookInRight",
		"eyeLookOutLeft",
		"eyeLookOutRight",
		"eyeLookUpLeft",
		"eyeLookUpRight",
		"eyeSquintLeft",
		"eyeSquintRight",
		"eyeWideLeft",
		"eyeWideRight",
		"jawForward",
		"jawLeft",
		"jawOpen",
		"jawRight",
		"mouthClose",
		"mouthDimpleLeft",
		"mouthDimpleRight",
		"mouthFrownLeft",
		"mouthFrownRight",
		"mouthFunnel",
		"mouthLeft",
		"mouthLowerDownLeft",
		"mouthLowerDownRight",
		"mouthPressLeft",
		"mouthPressRight",
		"mouthPucker",
		"mouthRight",
		"mouthRollLower",
		"mouthRollUpper",
		"mouthShrugLower",
		"mouthShrugUpper",
		"mouthSmileLeft",
		"mouthSmileRight",
		"mouthStretchLeft",
		"mouthStretchRight",
		"mouthUpperUpLeft",
		"mouthUpperUpRight",
		"noseSneerLeft",
		"noseSneerRight",
		"mouthPressLeft",
		"mouthPressRight",
		"tongueOut",
		"headYaw",
		"headPitch",
		"headRoll",
		"leftEyeYaw",
		"leftEyePitch",
		"leftEyeRoll",
		"rightEyeYaw",
		"rightEyePitch",
		"rightEyeRoll" };



	//2023年9月19日22:13:03 @LF
	//添加dll网络
	TArray<float> hasBodyData;

	TArray<float> bodyRoate;

	TArray<float> bodyPos;

	TArray<float> rootResult;

	TArray<float> lhandValid;

	TArray<float> lhandRoate;

	TArray<float> lhandPos;

	TArray<float> rhandValid;

	TArray<float> rhandRoate;

	TArray<float> rhandPos;

	float deltaTime;

protected:

	TFuture<void> AsyncTaskHandle;
	//virtual void PostLoad() override;
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "DSC|Json")
		bool GetDataArrField();

	void GetSocketNameList();

	UFUNCTION(BlueprintCallable, Category = "DSC|Function")
		TArray<FVector> GetLeftArrayTick();

	UFUNCTION(BlueprintCallable, Category = "DSC|Function")
		TArray<FVector> GetRightArrayTick();


	UFUNCTION(BlueprintCallable, Category = "DSC|Function")
		void ApplyFaceData(TArray<float>& FaceData);

	UFUNCTION(BlueprintCallable, Category = "DSC|Function")
		void ApplyRootLoc(const TArray<FVector>& LocArray);

	void CheckInit();

	UFUNCTION(BlueprintCallable, Category = "DSC|Function")
		int DLL_Begine();

	UFUNCTION(BlueprintCallable, Category = "DSC|Function")
		void DLL_Tick();


	void GetDLLInitDSPar(TArray<float>& arrPrt, TArray<float>& AuaPrt);

	void GetDLLInitHandPar(TArray<float>& handPtr, TArray<float>& handQua, int handType);

	/*针对metahuman等标准，手指会多出几个层级的处理方式*/
	void GetDLLInitHandAttach(TArray<float>& Curplam);

	void GetDLLInitBodyAttach(TArray<float>& Curplam);

	void DLLApplyBodyRot();

	void CheckHeight(float deltaTime);

	TArray<FRotator> GetBodyData();

	FName GetParentBoneName(const FName& ChildBoneName);


	static FCriticalSection gChangeMutex;

	bool bTickEnd = false;
	bool bHasBodyData = false;
	bool bHasLHandData = false;
	bool bHasRHandData = false;

};
