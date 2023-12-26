// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AnimGraphNode_Base.h"
#include "AnimNode_DSLiveLinkPose.h"
#include "AnimGraphNode_DSLiveLinkPose.generated.h"


UCLASS()
class ANIMATIONMNGEDITOR_API UAnimGraphNode_DSLiveLinkPose : public UAnimGraphNode_Base
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Settings)
	FAnimNode_DSLiveLinkPose Node;

public:
	//~ UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual FText GetMenuCategory() const;
	//~ End of UEdGraphNode

	//~ Begin UK2Node interface
	virtual void ConvertDeprecatedNode(UEdGraph* Graph, bool bOnlySafeChanges) override;
	virtual void GetOutputLinkAttributes(FNodeAttributeArray& OutAttributes) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	//~ End UK2Node interface
};
