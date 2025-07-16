#pragma once


#include "EdGraphUtilities.h"
#include "ChaRtEditor/PathGraph/Slate/SPathGraphNode.h"
#include "ChaRtEditor/PathGraph/EdGraph/PathEdGraphNode.h"



class FGraphChaRtNodeFactory : public FGraphPanelNodeFactory
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

// class FGraphChaRtPinFactory : public FGraphPanelPinFactory
// {
// 	virtual TSharedPtr<SGraphPin> CreatePin(class UEdGraphPin* Pin) const override
// 	{
// 		if (Pin != nullptr)
// 		{
// 			return 
// 		}
// 	}
// };