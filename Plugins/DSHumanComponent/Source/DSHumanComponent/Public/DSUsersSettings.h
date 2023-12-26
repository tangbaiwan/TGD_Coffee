// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "UObject/ObjectMacros.h"
#include "DSLibrary.h"
#include "DSUsersSettings.generated.h"

UENUM(BlueprintType)
enum class  BodySocket : uint8
{
	pelvis,
	spine_01,
	neck_01,
	head,
	upperarm_l,
	lowerarm_l,
	upperarm_r,
	lowerarm_r,
	thigh_l,
	calf_l,
	foot_l,
	ball_l,
	thigh_r,
	calf_r,
	foot_r,
	ball_r,
};

UENUM(BlueprintType)
enum class  LeftSocket : uint8
{
	hand_l,
	thumb_01_l,
	thumb_02_l,
	thumb_03_l,
	index_01_l,
	index_02_l,
	index_03_l,
	middle_01_l,
	middle_02_l,
	middle_03_l,
	ring_01_l,
	ring_02_l,
	ring_03_l,
	pinky_01_l,
	pinky_02_l,
	pinky_03_l,
	lowerarm_l,
};

UENUM(BlueprintType)
enum class  RightSocket : uint8
{
	hand_r,
	thumb_01_r,
	thumb_02_r,
	thumb_03_r,
	index_01_r,
	index_02_r,
	index_03_r,
	middle_01_r,
	middle_02_r,
	middle_03_r,
	ring_01_r,
	ring_02_r,
	ring_03_r,
	pinky_01_r,
	pinky_02_r,
	pinky_03_r,
	lowerarm_r,
};

USTRUCT()
struct FSockNameArray
{
	GENERATED_BODY()

	UPROPERTY(config, EditAnywhere, Category = "DS UserSettings", meta = (DisplayName = "当前方案名字"))
	FName OptionsName=TEXT("Metahum标准");

	UPROPERTY(config, EditAnywhere, Category = "DS UserSettings", meta = (DisplayName = "当前方案身体骨骼名称"))
		TMap<BodySocket,FName> BodySocketName = {
		{BodySocket::pelvis,FName("pelvis")},
		{BodySocket::spine_01 ,FName("spine_01")},
		{BodySocket::neck_01,FName("neck_01")},
		{BodySocket::head ,FName("head")},
		{BodySocket::upperarm_l,FName("upperarm_l")},
		{BodySocket::lowerarm_l,FName("lowerarm_l")},
		{BodySocket::upperarm_r,FName("upperarm_r")},
		{BodySocket::lowerarm_r,FName("lowerarm_r")},
		{BodySocket::thigh_l,FName("thigh_l")},
		{BodySocket::calf_l,FName("calf_l")},
		{BodySocket::foot_l,FName("foot_l")},
		{BodySocket::ball_l,FName("ball_l")},
		{BodySocket::thigh_r,FName("thigh_r")},
		{BodySocket::calf_r,FName("calf_r")},
		{BodySocket::foot_r,FName("foot_r")},
		{BodySocket::ball_r,FName("ball_r")},
	};

	UPROPERTY(config, EditAnywhere, Category = "DS UserSettings", meta = (DisplayName = "当前方案左手骨骼名称"))
		TMap<LeftSocket,FName> LeftSocketName = {
	{LeftSocket::hand_l,FName("hand_l")},
	{LeftSocket::thumb_01_l,FName("thumb_01_l")},
	{LeftSocket::thumb_02_l,FName("thumb_02_l")},
	{LeftSocket::thumb_03_l,FName("thumb_03_l")},
	{LeftSocket::index_01_l,FName("index_01_l")},
	{LeftSocket::index_02_l,FName("index_02_l")},
	{LeftSocket::index_03_l,FName("index_03_l")},
	{LeftSocket::middle_01_l,FName("middle_01_l")},
	{LeftSocket::middle_02_l,FName("middle_02_l")},
	{LeftSocket::middle_03_l,FName("middle_03_l")},
	{LeftSocket::ring_01_l,FName("ring_01_l")},
	{LeftSocket::ring_02_l,FName("ring_02_l")},
	{LeftSocket::ring_03_l,FName("ring_03_l")},
	{LeftSocket::pinky_01_l,FName("pinky_01_l")},
	{LeftSocket::pinky_02_l,FName("pinky_02_l")},
	{LeftSocket::pinky_03_l,FName("pinky_03_l")},
	{LeftSocket::lowerarm_l,FName("lowerarm_l")},
	};

	UPROPERTY(config, EditAnywhere, Category = "DS UserSettings", meta = (DisplayName = "当前方案右手骨骼名称"))
		TMap<RightSocket,FName> RightSocketName{
		{RightSocket::hand_r,FName("hand_r")},
		{RightSocket::thumb_01_r,FName("thumb_01_r")},
		{RightSocket::thumb_02_r,FName("thumb_02_r")},
		{RightSocket::thumb_03_r,FName("thumb_03_r")},
		{RightSocket::index_01_r,FName("index_01_r")},
		{RightSocket::index_02_r,FName("index_02_r")},
		{RightSocket::index_03_r,FName("index_03_r")},
		{RightSocket::middle_01_r,FName("middle_01_r")},
		{RightSocket::middle_02_r,FName("middle_02_r")},
		{RightSocket::middle_03_r,FName("middle_03_r")},
		{RightSocket::ring_01_r,FName("ring_01_r")},
		{RightSocket::ring_02_r,FName("ring_02_r")},
		{RightSocket::ring_03_r,FName("ring_03_r")},
		{RightSocket::pinky_01_r,FName("pinky_01_r")},
		{RightSocket::pinky_02_r,FName("pinky_02_r")},
		{RightSocket::pinky_03_r,FName("pinky_03_r")},
		{RightSocket::lowerarm_r,FName("lowerarm_r")},
	};
};


UCLASS(config = DSUserSettings, meta = (DisplayName = "DS UserSettings"))
class DSHUMANCOMPONENT_API UDSUsersSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UDSUsersSettings();

	/** The amount of animation (in seconds) to stream ahead of time (per stream) */
	UPROPERTY(config, EditAnywhere, Category = "DS UserSettings")
		FString appID;

	UPROPERTY(config, EditAnywhere, Category = "DS UserSettings")
		FString appSecret;

	UPROPERTY(config, EditAnywhere, Category = "DS UserSettings")
		FString IP;

	UPROPERTY(config, EditAnywhere, Category = "DS UserSettings", meta = (DisplayName = "端口", ClampMin = "0", ClampMax = "65535"))
		int32 Port;

	UPROPERTY(config, EditAnywhere, Category = "DS UserSettings", meta = (DisplayName = "采集模式选择"))
		EDataOptions DataOptions;

	UPROPERTY(config, EditAnywhere, Category = "DS UserSettings", meta = (DisplayName = "是否锁帧"))
		bool bUseFixedFrameRate;

	/*默认建议帧率为30，帧率越高，越影响GPU,导致其它程序效率低，比如采集端*/
	UPROPERTY(config, EditAnywhere, Category = "DS UserSettings", meta = (DisplayName = "帧率", ClampMin = "10", ClampMax = "120"))
		int32 FixedFrameRate;

	UPROPERTY(config, EditAnywhere, Category = "DS UserSettings|Dev Setting", meta = (DisplayName = "手腕平滑最小值", ClampMin = "0", ClampMax = "200"))
		float MinFloat;

	UPROPERTY(config, EditAnywhere, Category = "DS UserSettings|Dev Setting", meta = (DisplayName = "手腕平滑最大值", ClampMin = "0", ClampMax = "200"))
		float MaxFloat;

	UPROPERTY(config, EditAnywhere, Category = "DS UserSettings|Dev Setting", meta = (DisplayName = "手指平滑最大值", ClampMin = "0", ClampMax = "200"))
		float FigMaxFloat;

	UPROPERTY(config, EditAnywhere, Category = "DS UserSettings|Dev Setting", meta = (DisplayName = "手指平滑最小值", ClampMin = "0", ClampMax = "200"))
		float FigMinFloat;


	UPROPERTY(config, EditAnywhere, Category = "DS UserSettings|Dev Setting", meta = (DisplayName = "角色鼻子系数", ClampMin = "-10", ClampMax = "10"))
		float floatpy;

	UPROPERTY(config, EditAnywhere, Category = "Socket Setting", meta = (DisplayName = "选择骨骼匹配方案索引"))
	uint8 CurSelectedOptions=0;

	UPROPERTY(config, EditAnywhere, Category = "Socket Setting", meta = (DisplayName = "骨骼匹配方案库"))
	TArray<FSockNameArray> AllSockNameArray;

};
