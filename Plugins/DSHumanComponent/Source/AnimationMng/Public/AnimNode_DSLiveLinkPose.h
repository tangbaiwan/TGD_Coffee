// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Animation/AnimNodeBase.h"
#include "CoreMinimal.h"
#include "DSActorComponent.h"
#include "LiveLinkRetargetAsset.h"
#include "LiveLinkTypes.h"
#include "AnimNode_DSLiveLinkPose.generated.h"

class ILiveLinkClient;


//PRAGMA_DISABLE_DEPRECATION_WARNINGS
//UCLASS(meta = (Keywords = "Modify Body"))
USTRUCT(BlueprintInternalUseOnly)
struct ANIMATIONMNG_API FAnimNode_DSLiveLinkPose : public FAnimNode_Base
{
	GENERATED_USTRUCT_BODY()

public:

	class UDSActorComponent* DSActorPtr = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		FPoseLink InputPose;

	/** Type of blending used (Linear, Cubic, etc.) */
	UPROPERTY(EditAnywhere, Category = "Blend")
		EAlphaBlendOption BlendOption= EAlphaBlendOption::Linear;

	UPROPERTY(EditAnywhere, Category = "Blend")
		TObjectPtr<UCurveFloat> CustomCurve;

	FLiveLinkSubjectName LiveLinkSubjectName;

	int32 FrameValue = 0;

	int8 FrameValueNumber = 0;

	TArray<FLiveLinkSubjectKey> CurrentFLiveLinkSubjectKeys;

	TSubclassOf<ULiveLinkRetargetAsset> RetargetAsset;

	UPROPERTY(transient)
		TObjectPtr<ULiveLinkRetargetAsset> CurrentRetargetAsset;

	// The animation sequence asset to evaluate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (PinShownByDefault, DisplayName = "重定向Pose"))
		TObjectPtr<UPoseAsset> PoseAsset;

	TWeakObjectPtr<UPoseAsset> CurrentPoseAsset;

	FAnimExtractContext PoseExtractContext;

	TArray<FName> HumanFace;

	TMap<FName, FName> Face2Face;

	//暂时用不上
	TArray<float> BoneBlendWeights;

public:
	FAnimNode_DSLiveLinkPose();

	//~ FAnimNode_Base interface
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
	virtual void Update_AnyThread(const FAnimationUpdateContext& Context) override;
	virtual void Evaluate_AnyThread(FPoseContext& Output) override;
	virtual bool HasPreUpdate() const { return true; }
	virtual void PreUpdate(const UAnimInstance* InAnimInstance) override;

	bool Serialize(FArchive& Ar);

	int32 GetStringSub(const FString& SearchIn, const FString& Substring, bool bUseCase, bool bSearchFromEnd);

	void RebuildPoseList(const FBoneContainer& InBoneContainer, const UPoseAsset* InPoseAsset, FAnimExtractContext& PoseEC);

private:

	ILiveLinkClient* LiveLinkClient_AnyThread;

	// Delta time from update so that it can be passed to retargeter
	float CachedDeltaTime;
};

//PRAGMA_ENABLE_DEPRECATION_WARNINGS


//template<> struct TStructOpsTypeTraits<FAnimNode_DSLiveLinkPose> : public TStructOpsTypeTraitsBase2<FAnimNode_DSLiveLinkPose>
//{
//	enum
//	{
//		WithSerializer = true
//	};
//};
