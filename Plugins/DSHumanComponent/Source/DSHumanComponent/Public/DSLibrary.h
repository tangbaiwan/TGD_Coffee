// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DSLibrary.generated.h"


//DECLARE_LOG_CATEGORY_EXTERN(LogTemp, Log, All);
//DEFINE_LOG_CATEGORY_STATIC(LogTemp, Log, All)
/**
 *
 */

UENUM(BlueprintType)
enum class EDataOptions :uint8
{
	monocular		UMETA(DisplayName = "单目采集"),
	binocular		UMETA(DisplayName = "双目采集"),
};

//DEFINE_LOG_CATEGORY(DSLog);

UENUM(BlueprintType)
enum class ELinkOptions :uint8
{

	DSLink,
	LiveLink,
	Both,
};

UENUM()
enum class LandmarkType : uint8
{
	/** Package was saved successfully */
	LeftHand,
	RightHand,
};

static TArray<FName> DLLBodySocketName = {
	FName("pelvis"), FName("1"),
	FName("spine_01"), FName("1"),
	FName("neck_01"), FName("1"),
	FName("head"), FName("1"),
	FName("nose"), FName("0"),

	FName("upperarm_l"), FName("1"),
	FName("lowerarm_l"), FName("1"),
	FName("hand_l"), FName("0"),
	FName("index_01_l"), FName("0"),
	FName("ring_01_l"), FName("0"),

	FName("upperarm_r"), FName("1"),
	FName("lowerarm_r"), FName("1"),
	FName("hand_r"), FName("0"),
	FName("index_01_r"), FName("0"),
	FName("ring_01_r"), FName("0"),

	FName("thigh_l"), FName("1"),
	FName("calf_l"), FName("1"),
	FName("foot_l"), FName("1"),
	FName("ball_l"), FName("0"),

	FName("thigh_r"), FName("1"),
	FName("calf_r"), FName("1"),
	FName("foot_r"), FName("1"),
	FName("ball_r"), FName("0")
};

static TArray<FName> DLLLeftSocketName = {
	FName("hand_l"),
	FName("thumb_01_l"),
	FName("thumb_02_l"),
	FName("thumb_03_l"),
	FName("index_01_l"), 
	FName("index_02_l"), 
	FName("index_03_l"), 
	FName("middle_01_l"),
	FName("middle_02_l"),
	FName("middle_03_l"),
	FName("ring_01_l"), 
	FName("ring_02_l"), 
	FName("ring_03_l"), 
	FName("pinky_01_l"),
	FName("pinky_02_l"),
	FName("pinky_03_l"),
	FName("lowerarm_l"),
};

static TArray<FName> DLLRightSocketName= {
	FName("hand_r"),
	FName("thumb_01_r"),
	FName("thumb_02_r"),
	FName("thumb_03_r"),
	FName("index_01_r"),
	FName("index_02_r"),
	FName("index_03_r"),
	FName("middle_01_r"), 
	FName("middle_02_r"), 
	FName("middle_03_r"), 
	FName("ring_01_r"),
	FName("ring_02_r"),
	FName("ring_03_r"),
	FName("pinky_01_r"), 
	FName("pinky_02_r"), 
	FName("pinky_03_r"), 
	FName("lowerarm_r"), 
};



UCLASS()
class DSHUMANCOMPONENT_API UDSLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "DS|Function")
		static	int64 GetTimesTamp();

	UFUNCTION(BlueprintPure, Category = "DS|Function")
		static	FString Int64TofString(const int64 Time);

	UFUNCTION(BlueprintPure, Category = "DS|Function")
		static	FString GetHashMD5(const FString& Parameters, const FString& Secret);


	UFUNCTION(BlueprintCallable, Category = "DS|File IO")
		static bool LoadStringFromFile(FString& Result, FString FullFilePath = "Enter Full File Path");

	UFUNCTION(BlueprintCallable, Category = "DS|File IO")
		static bool SaveStringToFile(FString Result, FString FullFilePath = "Enter Full File Path", bool bAppend = false);


	UFUNCTION(BlueprintCallable, Category = "DS|Function")
		static FDateTime GetNowUtc();

	UFUNCTION(BlueprintCallable, Category = "DS|Function")
		static	float GetTimeDifferenceInSeconds(FDateTime StartDateTime);


	UFUNCTION(BlueprintPure, Category = "DS|Function")
		static 	FString EncryptVersion(const FString& Content, const FString& key);


	UFUNCTION(BlueprintPure, Category = "DS|Function")
		static FString GetLocalIPAddress();

	void UEPrint(FString* Content);

	template<typename T>
	static void UEPrintValue(FString Prefix, const T& Value)
	{
		FString ValueString = LexToString(Value);
		FString LogMessage = Prefix + FString::Printf(TEXT(": %s"), *ValueString);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *LogMessage);
	}
};