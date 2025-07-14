// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraphUtilities.h"
#include "EdGraph/EdGraphNode.h"
#include "GraphEditor/Slate/PathGraph/SPathGraphNode.h"
#include "PathEdGraphNode.generated.h"

/**
 * 
 */

UCLASS()
class GRAPHCHARTEDITOR_API UPathEdGraphNode : public UEdGraphNode
{
	GENERATED_BODY()

public:

	UPathEdGraphNode();

	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
};

class FPathEdGraphNodeFactory : public FGraphPanelNodeFactory
{
	virtual TSharedPtr<SGraphNode> CreateNode(class UEdGraphNode* Node) const override
	{
		if (UPathEdGraphNode* PathEdGraphNode = Cast<UPathEdGraphNode>(Node))
		{
			return SNew(SPathGraphNode, PathEdGraphNode);
		}
		return nullptr;
	}
};
