// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "BoneContainer.h"
#include "BonePose.h"
#include "AnimNodeBaseCtrl.h"

#include "AnimNodeBody.generated.h"

class USkeletalMeshComponent;

struct FCommonBoneNode
{
	FName sName = "";
	FCommonBoneNode* pParentNode = nullptr;
	float fLenth = 0.0f;
};

UENUM(BlueprintType)
enum EBoneModificationModeEx
{
	/** The modifier ignores this channel (keeps the existing bone translation, rotation, or scale). */
	BMM_Ignore UMETA(DisplayName = "Ignore"),

	/** The modifier replaces the existing translation, rotation, or scale. */
	BMM_Replace UMETA(DisplayName = "Replace Existing"),

	/** The modifier adds to the existing translation, rotation, or scale. */
	BMM_Additive UMETA(DisplayName = "Add to Existing")
};

/**
 *	Simple controller that replaces or adds to the translation/rotation of a single bone.
 */
USTRUCT(BlueprintInternalUseOnly)
struct ANIMATIONMNG_API FAnimNodeBody : public FAnimNodeBaseCtrl
{
	GENERATED_USTRUCT_BODY()

	class UDSActorComponent* DSActorPtr = nullptr;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinShownByDefault))
	AActor* parentActor = nullptr;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinShownByDefault))
		USkeletalMeshComponent* bodyComp = nullptr;

	/*(Beta)偏移位置，!!当前版本仅仅Location生效，*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinShownByDefault, DisplayName = "偏移位置(Beta)"))
		FTransform Offest;

	FVector RootLocOffest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (NeverAsPin))
		bool enabled = true;

	/*持续开发节点，暂时不建议勾选*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (NeverAsPin))
		bool bUseUE = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinShownByDefault))
		bool hasLeft = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl, meta = (PinShownByDefault))
		bool hasRight = true;


	bool bHasDsActorData = false;

	bool hasLeft1 = true;
	TArray<FRotator> handRotL;

	bool hasRight1 = true;
	TArray<FRotator> handRotR;
	TArray<FVector> CurrenthandData;

	TArray<FName> aAllBody =
	{
		"pelvis",
		"spine_01",
		"neck_01",
		"head",
		"upperarm_l",
		"lowerarm_l",
		"upperarm_r",
		"lowerarm_r",
		"thigh_l",
		"calf_l",
		"foot_l",
		"thigh_r",
		"calf_r",
		"foot_r",
	};

	TArray<FName> aAllLeftHand =
	{
		"Hand_L",
		"thumb_01_l",
		"thumb_02_l",
		"thumb_03_l",
		"index_01_l",
		"index_02_l",
		"index_03_l",
		"middle_01_l",
		"middle_02_l",
		"middle_03_l",
		"ring_01_l",
		"ring_02_l",
		"ring_03_l",
		"pinky_01_l",
		"pinky_02_l",
		"pinky_03_l"
	};

	TArray<FName> aAllRightHand =
	{
		"Hand_R",

		"thumb_01_R",
		"thumb_02_R",
		"thumb_03_R",
		"index_01_R",
		"index_02_R",
		"index_03_R",
		"middle_01_R",
		"middle_02_R",
		"middle_03_R",
		"ring_01_R",
		"ring_02_R",
		"ring_03_R",
		"pinky_01_R",
		"pinky_02_R",
		"pinky_03_R"
	};

	TArray<FBoneReference> aBodyRef,aLeftHandRef, aRightHandRef;
	TArray<FRotator> aBodyOld,aLeftHandOld, aRightHandOld;
	TArray<FRotator> aLeftHandOldAngle, aRightHandOldAngle;
	TArray<FVector> tempLeftData, tempRightData;
	TArray<FRotator> tempBodyData;
	/** Name of bone to control. This is the main bone chain to modify from. **/
	//UPROPERTY(EditAnywhere, Category=SkeletalControl) 
	FBoneReference BoneToModify;

	/** New translation of bone to apply. */
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Translation, meta=(PinShownByDefault))
	FVector Translation;

	/** New rotation of bone to apply. */
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Rotation, meta=(PinShownByDefault))
	FRotator Rotation;

	/** New Scale of bone to apply. This is only worldspace. */
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Scale, meta=(PinShownByDefault))
	FVector Scale;

	/** Whether and how to modify the translation of this bone. */
	//UPROPERTY(EditAnywhere, Category=Translation)
	TEnumAsByte<EBoneModificationModeEx> TranslationMode;

	/** Whether and how to modify the translation of this bone. */
	//UPROPERTY(EditAnywhere, Category=Rotation)
	TEnumAsByte<EBoneModificationModeEx> RotationMode;

	/** Whether and how to modify the translation of this bone. */
	//UPROPERTY(EditAnywhere, Category=Scale)
	TEnumAsByte<EBoneModificationModeEx> ScaleMode;

	/** Reference frame to apply Translation in. */
	//UPROPERTY(EditAnywhere, Category=Translation)
	TEnumAsByte<enum EBoneControlSpace> TranslationSpace;

	/** Reference frame to apply Rotation in. */
	//UPROPERTY(EditAnywhere, Category=Rotation)
	TEnumAsByte<enum EBoneControlSpace> RotationSpace;

	/** Reference frame to apply Scale in. */
	//UPROPERTY(EditAnywhere, Category=Scale)
	TEnumAsByte<enum EBoneControlSpace> ScaleSpace;

	FAnimNodeBody();

	// FAnimNode_Base interface
	virtual void GatherDebugData(FNodeDebugData& DebugData) override;
	// End of FAnimNode_Base interface

	// FAnimNode_SkeletalControlBase interface
	virtual void EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms) override;
	virtual bool IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones) override;
	// End of FAnimNode_SkeletalControlBase interface

private:
	// FAnimNode_SkeletalControlBase interface
	virtual void InitializeBoneReferences(const FBoneContainer& RequiredBones) override;

	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context)override;

	void DsBeginPlay(FComponentSpacePoseContext& Output);

	// End of FAnimNode_SkeletalControlBase interface
	void ModifyBone(FCompactPoseBoneIndex& index, FComponentSpacePoseContext& poseContext, TArray<FBoneTransform>& outTrans, FRotator rotation,
		TEnumAsByte<enum EBoneControlSpace> rotSpace = EBoneControlSpace::BCS_ComponentSpace, TEnumAsByte<EBoneModificationModeEx> rotMode = EBoneModificationModeEx::BMM_Replace,
		FVector translation = FVector::ZeroVector, TEnumAsByte<enum EBoneControlSpace> strsSpace = EBoneControlSpace::BCS_ComponentSpace,
		TEnumAsByte<EBoneModificationModeEx> transMode = EBoneModificationModeEx::BMM_Ignore);

	void InitBone();

	void FixedHand(FComponentSpacePoseContext& Output,bool bFixedLeft);
};
